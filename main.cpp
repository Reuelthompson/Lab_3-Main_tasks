#include "mbed.h"
#include "arm_book_lib.h"

int main()
{
    DigitalIn enterButton(BUTTON1);     //Initialise Buttons
    DigitalIn gasDetector(D2);
    DigitalIn overTempDetector(D3);
    DigitalIn aButton(D4);
    DigitalIn bButton(D5);
    DigitalIn cButton(D6);
    DigitalIn dButton(D7);

    DigitalOut alarmLed(LED1);          //Initialise LEDs
    DigitalOut incorrectCodeLed(LED3);
    DigitalOut systemBlockedLed(LED2);

    gasDetector.mode(PullDown);         // dictate pull down or pull up connecction
    overTempDetector.mode(PullDown);
    aButton.mode(PullDown);
    bButton.mode(PullDown);
    cButton.mode(PullDown);
    dButton.mode(PullDown);

    alarmLed = OFF;                     //Set initial state of LEDs
    incorrectCodeLed = OFF;
    systemBlockedLed = OFF;

    int alarmState = OFF;
    int numberOfIncorrectCodes = 0;

    while (true) {                              //main loop to keep checking for an input
        if (gasDetector && overTempDetector){   //If both are detected run the next bit of code
            alarmState = ON;                    

            while(alarmState == ON){            //Loop until the alarm state changes to false
                alarmLed = !alarmLed;
                rtos::ThisThread::sleep_for(100ms); //Command to set the LED to blink

                if ( numberOfIncorrectCodes < 5 ) { //Check to see if the number of incorrect codes is less than 5
            
                    if ( aButton && bButton && cButton && dButton && !enterButton ) { // incorrect code reset
                    incorrectCodeLed = OFF;
                }
                    if ( enterButton && !incorrectCodeLed && alarmState) { 
                        if ( aButton && bButton && !cButton && !dButton ) { // passcode combination
                            alarmState = OFF;
                            numberOfIncorrectCodes = 0;
                        } else {                                            //If incorrect password is input then turn LED on and add 1 to incorrect codes
                            incorrectCodeLed = ON;
                            numberOfIncorrectCodes = numberOfIncorrectCodes + 1;
                        }
                    }
                } else {
                    for(int I = 0; I<120; I++){                 //for loop that goes for 120 500ms intervals turning the LED on and off
                        systemBlockedLed = !systemBlockedLed;
                        rtos::ThisThread::sleep_for(500ms);
                    }
                systemBlockedLed = OFF;                         //After the 60sec wait the user can go again because the codes is reset
                numberOfIncorrectCodes = 0;
                }
            }
        }
        if ( gasDetector || overTempDetector ) {                // if gas or overtemp are on
            alarmState = ON;
        }

        alarmLed = alarmState;

        if ( numberOfIncorrectCodes < 5 ) {
            
            if ( aButton && bButton && cButton && dButton && !enterButton ) {
                incorrectCodeLed = OFF;
            }

            if ( enterButton && !incorrectCodeLed && alarmState) {
                if ( aButton && bButton && !cButton && !dButton ) {
                    alarmState = OFF;
                    numberOfIncorrectCodes = 0;
                } else {
                    incorrectCodeLed = ON;
                    numberOfIncorrectCodes = numberOfIncorrectCodes + 1;
                }
            }
        } else {
            for(int I = 0; I<120; I++){
                    systemBlockedLed = !systemBlockedLed;
                    rtos::ThisThread::sleep_for(500ms);
                }
        systemBlockedLed = OFF;
        numberOfIncorrectCodes = 0;
            }
        }
    }
