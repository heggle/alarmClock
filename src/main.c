//This program uses the RTC to display time on a 4 digit 7 segment display
//When the alarm triggers, it plays mp3 files through a USB connected on the
//micro USB port

#include "stm32f4xx_rtc.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_pwr.h"
#include "stm32f4xx_dac.h"
#include "stm32f4xx_tim.h"
#include "misc.h"
#include "stm32f4xx_exti.h"
#include "audioMP3.h"
#include "timeKeeping.h"
#include "main.h"


#include<stdio.h>
#include<stdlib.h>

//structures
RCC_ClocksTypeDef RCC_Clocks;
GPIO_InitTypeDef    GPIOInitStruct;
TIM_TimeBaseInitTypeDef TIM_InitStruct;
NVIC_InitTypeDef NVIC_InitStructure;
EXTI_InitTypeDef EXTI_InitStructure;
I2C_InitTypeDef I2C_InitStruct;


//function prototypes
void configuration(void);
void display7Seg(int hours, int minutes, RTC_TimeTypeDef amPm);
void setTime(void);
void setAlarm(void);
void snooze(void);
void getCurrentTime(void);
int convert24Hour(int hours, RTC_TimeTypeDef amPm);
void addHour(RTC_TimeTypeDef * timeStruct);
void addMin(RTC_TimeTypeDef * timeStruct);
void buttonControls(void);
uint8_t bcd(uint8_t dec);
uint8_t decimal(uint8_t bcd);
void amPmToggle(RTC_TimeTypeDef * timeStruct);


//Flag for each pushbutton -- necessary for debouncing
int buttonFlagC6 = 0;
int buttonFlagC2 = 0;
int buttonFlagC1 = 0;
int buttonFlagC5 = 0;
int buttonFlagC8 = 0;
int buttonFlagC9 = 0;



//global variables
int time[4];
int interruptOccurred = 0;
int AlarmPmFlag = 0;
int TimePmFlag = 0;
int hour24Flag = 0;
unsigned int debouncing = 0;
int buttonState = 0;
int digit;
//int number;
uint8_t number;
int TIMEBUTTON = 0;		//(GPIOC->IDR & GPIO_Pin_1)
int TIMEUPBUTTON = 0;	//(GPIOC->IDR & GPIO_Pin_2)
int ALARMBUTTON = 0;		//(GPIOC->IDR & GPIO_Pin_5)
int MODEBUTTON = 0;	//(GPIOC->IDR & GPIO_Pin_6)
int RESETBUTTON	= 0;	//(GPIOC->IDR & GPIO_Pin_8)
int SNOOZEBUTTON = 0;		//(GPIOC->IDR & GPIO_Pin_9)

//Counters to see how many times a button has been pressed
int buttonPressed = 0;
int alarmButtonPressed = 0;
int snoozeButtonPressed = 0;

//Flags for the first initializing the set time or set alarm functions.
int tempTimeSet = 0;
int tempAlarmSet = 0;

//Display-related flags
int minFlash = 0;//Flags added to flash the display
int hoursFlash = 0;
int amPmFlash = 0;
int flashCounter = 0; //this is going to determine the delay of the flash
int flashOn = 0;

//Temporary variables for time and alarm setting
RTC_TimeTypeDef tempSetTime;
//24 hour time struct for displaying 24 hour time
int digitDisplayCounter = 0;

/*
extern volatile int exitMp3 = 0;
extern volatile int mp3PlayingFlag = 0;
extern volatile int snoozeMemory = 0;
*/

/*for testing
uint32_t *ptr;
uint32_t *ptr2;*/


//volatile uint8_t Hours = myclockTimeStruct.RTC_Hours;
//volatile uint8_t Minutes = myclockTimeStruct.RTC_Hours;


int main(void)
{

 	configuration();

	// This flag should be set to zero AFTER you have debugged your MP3 player code
	// In your final code, this will be set to 1 when the alarm time is reached in the
	// RTC_ALARM_IRQHandler function.
	// It is set to one here for testing purposes only.

	//interruptOccurred = 1;

	//continuously loops checking for the alarm interrupt
	while(1)
	{
		   //mp3PlayingFlag = 1;
		   //audioToMp3();

		//checks for alarm A interrupt and calls the mp3 function
		if(1 == interruptOccurred && mp3PlayingFlag == 0)
		{

			interruptOccurred = 0;
			mp3PlayingFlag = 1;
			audioToMp3();
		}

		//ensures the alarm is not already playing
		else if(1 == interruptOccurred && mp3PlayingFlag == 1)
		{
			interruptOccurred = 0;
		}

	}

}

//timer interrupt handler that is called at a rate of 500Hz
//this function gets the time and displays it on the 7 segment display
//it also checks for button presses, debounces, and handles each case
void TIM5_IRQHandler(void)
{
	//int previousState = 0;


	//checks to see if the mp3 is playing and turns on the motor
	if (mp3PlayingFlag == 1) {
		GPIO_SetBits(GPIOC, GPIO_Pin_11);
	} else {
		GPIO_ResetBits(GPIOC, GPIO_Pin_11);
	}



	//make sure alarm set in config is enabled
	RTC_AlarmCmd(RTC_Alarm_A,ENABLE);

	//double checks that interrupt has occurred
	if( TIM_GetITStatus( TIM5, TIM_IT_Update ) != RESET )
	{

		if(  buttonPressed > 0 ){

			setTime();
		}

		getCurrentTime();

		buttonControls();

		//Display
	    if ((TIMEBUTTON ==1 && tempTimeSet > 0) || (ALARMBUTTON == 1 && tempAlarmSet > 0)) {
		     display7Seg(tempSetTime.RTC_Hours, tempSetTime.RTC_Minutes, tempSetTime);
	    } else { //Display the actual time
		     display7Seg(myclockTimeStruct.RTC_Hours, myclockTimeStruct.RTC_Minutes, myclockTimeStruct);
	    }


		//if snooze button is pressed cancels the mp3 and sets alarm 10 minutes later
		if(SNOOZEBUTTON == 1)
		{
			if(mp3PlayingFlag == 1)
			{
				exitMp3 = 1;
				snoozeButtonPressed++;
				snooze();
			}
		}

		//if the reset button is pressed without mp3 playing, it turns off and on the alarm
		//otherwise it cancels the mp3
		if(1 == RESETBUTTON)
		{
			if(mp3PlayingFlag == 0)  //if not playing
			{

				//toggle alarm
				if(0 != (0x100 & RTC->CR))  //if alarm is on
				{
					RTC_AlarmCmd(RTC_Alarm_A,DISABLE);

				}

				//if alarm is off
				else
				{
					RTC_AlarmCmd(RTC_Alarm_A,ENABLE);
					RTC_ClearFlag(RTC_FLAG_ALRAF);
				}

			} else {
				RTC_AlarmCmd(RTC_Alarm_A,DISABLE);
				RTC_ClearFlag(RTC_FLAG_ALRAF);
				exitMp3 = 1;

			}

			//checks if snooze has been used before, resets the alarm to original time
			if (snoozeButtonPressed > 0)
			{
				AlarmStruct.RTC_AlarmTime = alarmMemory.RTC_AlarmTime;
				RTC_SetAlarm(RTC_Format_BCD,RTC_Alarm_A,&AlarmStruct);
				snoozeButtonPressed = 0;
			}

		}


		//works similar to the process of setting the time, but works when pushing the set alarm button
		if(alarmButtonPressed > 0)
		{
			/*
			//checks to see if the alarm was on or off previously
			if(0 != (0x100 & RTC->CR))
			{
				previousState = 1;
			}
			*/
			setAlarm();
			/*
			//if the alarm was on, it is re-enabled
			if(1==previousState)
			{
				RTC_AlarmCmd(RTC_Alarm_A,ENABLE);
				RTC_ClearFlag(RTC_FLAG_ALRAF);
				previousState = 0;
			}*/

		}

		//clears interrupt flag
		TIM5->SR = (uint16_t)~TIM_IT_Update;

	}
}

//alarm A interrupt handler
//when alarm occurs, clear all the interrupt bits and flags
//then set the flag to play mp3
void RTC_Alarm_IRQHandler(void)
{
	//resets alarm flags and sets flag to play mp3
	if(RTC_GetITStatus(RTC_IT_ALRA) != RESET)
	{
		RTC_ClearFlag(RTC_FLAG_ALRAF);
		RTC_ClearITPendingBit(RTC_IT_ALRA);
		EXTI_ClearITPendingBit(EXTI_Line17);
		interruptOccurred = 1;

	}
}

//displays the current clock or alarm time
void display7Seg(int hours, int minutes, RTC_TimeTypeDef amPm)
{
	hours = decimal(hours);
	minutes = decimal(minutes);
	if(MODEBUTTON == 1) {
		hours = convert24Hour(hours, amPm);
	}
	// Note that writing to D4 will affect communication with CS43L22 chip.
	// Writing to D15,D14,D13 will affect the indicator LEDs on the STM32F4 board.

	//Flash control
	if (flashCounter < 400) {
		flashOn = 1;
	} else {
		flashOn = 0;
	}

	if (flashCounter > 550) flashCounter = 0;
	flashCounter++;


	//Reset all the segments

	GPIO_ResetBits(GPIOA, GPIO_Pin_8);
	GPIO_ResetBits(GPIOB, GPIO_Pin_4);
	GPIO_ResetBits(GPIOB, GPIO_Pin_7);
	GPIO_ResetBits(GPIOE, GPIO_Pin_11);
	GPIO_ResetBits(GPIOD, GPIO_Pin_3);
	GPIO_ResetBits(GPIOD, GPIO_Pin_7);
	GPIO_ResetBits(GPIOE, GPIO_Pin_8);

	//Reset all the digits

	GPIO_ResetBits(GPIOA, GPIO_Pin_10);
	GPIO_ResetBits(GPIOE, GPIO_Pin_10);
	GPIO_ResetBits(GPIOE, GPIO_Pin_14);
	GPIO_ResetBits(GPIOB, GPIO_Pin_10);
	GPIO_ResetBits(GPIOB, GPIO_Pin_14); //colon

	//"Cathode Pins" - Give the following digits or colon the potential to be displayed if an segment anode is connected
	//    digit 1 - A10
	//    digit 2 - E10
	//    digit 3 - E14
	//    digit 4 - B10
	//    digit 5 - colon and 'L3' dot - B14


	//"Anode Pins" - Light up the corresponding segment of any digits that have their "cathode pins" connected
	//    a - B4
	//    b - D3
	//    c - B7
	//    d - B2
	//    e - E8
	//    f - A8
	//    g - D7
	//  top colon dot - B4 (same as 'a')
	//  bottom colon dot - D3 (same as 'b')
	// 'L3' dot - B7 (same as 'c')
	// 'DP' dot after digits -- NEED TO CONNECT a wire to one of the spare ports in LED PCB


	// 0 - a,b,c,d,e,f,     |A8 B7 B4 D3 E8 B2
	// 1 - e,f              |A8 E8
	// 2 - a,b,g,e,d        |B4 D7 D3 E8 B2
	// 3 - a,b,c,d,g        |B7 B4 D7 D3 B2
	// 4 - b,c,f,g          |A8 B7 D7 D3
	// 5 - a,c,d,f,g        |A8 B7 B4 D7 B2
	// 6 - a,c,d,e,f,g      |A8 B7 B4 D7 E8 B2
	// 7 - a,b,c            |B7 B4 D3
	// 8 - a,b,c,d,e,f,g    |A8 B7 B4 D7 D3 E8 B2
	// 9 - a,b,c,d,f,g      |A8 B7 B4 D7 D3 B2

	//int skip = 0; //local flag to indicate that display should be skipped if we are on the "off" flash period

	//Cycle through displaying 4 digits and the colon
	int digit = digitDisplayCounter;
	digitDisplayCounter++;
	if (digitDisplayCounter >5) digitDisplayCounter = 0;

	/* BLINKING CODE-----------
        // If we are in hourflashing mode and we are supposed to be displaying the first or second digit
        if ((hoursFlash == 1 && digit = 0) || (hoursFlash ==1 && digit == 1)){
            if (flashCounter < 100 ) { //Setting 100 counts as the trial flash time
                if (flashBlinkState == 0){ //Currently in the "off" state of blinking
                    skip = 1; //Set flag to skip displaying the next digit 
                }
            } else { //if flash counter greater than 100
                flashCounter = 0; //Reset the counter to zero
                //toggle the flashBlinkState flag to the other state
                if (flashBlinkState = 0)  {
                    flashBlinkState = 1;//the "on" state of blinking
                }else {
                    flashBlinkState = 0
                }
            }
        }

        // If we are in min flashing mode and we are supposed to be displaying the third or fourth digit
        if (minFlash == 1 && digit = 2|| minFlash ==1 && digit == 3){
            if (flashCounter < 100 ) { //Setting 100 counts as the trial flash time
                if (flashBlinkState == 0){ //Currently in the "off" state of blinking
                    skip = 1; //Set flag to skip displaying this digit
                }
            } else { //if flash counter greater than 100
                flashCounter = 0; //Reset the counter to zero
                //toggle the flashBlinkState flag to the other state
                if (flashBlinkState = 0)  {
                    flashBlinkState = 1; //the "on" state of blinking
                }else {
                    flashBlinkState = 0
                }
            }
        }
	 */

	//disable first digit for 12 hour times less than 10 AND AM/PM dot
	//and alarm set dot

	//digit =3;

	//  if (skip != 1){ //if we are not in the "off" state of blinking then proceed
	//      otherwise, skip this code below because we don't want to display anything!



	switch (digit){
	case 0: //digit 1
		//Need the first digit
		number = hours / 10;

		//flash control
		if (hoursFlash == 0 || (flashOn == 1 && hoursFlash == 1)){
			// If its in 12 hour mode and the number is 0, do not display it (leading zero)
			if (number > 0 && MODEBUTTON == 0) {
				GPIO_SetBits(GPIOA, GPIO_Pin_10);
			} else if (MODEBUTTON == 1) {
				GPIO_SetBits(GPIOA, GPIO_Pin_10);
			}
		}

		break;
	case 1: //digit 2
		number = hours % 10;
		if (hoursFlash == 0 || (flashOn == 1 && hoursFlash == 1)){
			GPIO_SetBits(GPIOE, GPIO_Pin_10);
		}
		break;
	case 2: //digit 3
		//Turn on pin B10
		number = minutes / 10;
		if (minFlash == 0 || (flashOn == 1 && minFlash == 1)){
			GPIO_SetBits(GPIOE, GPIO_Pin_14);
		}
		break;
	case 3: //digit 4

		number = minutes % 10;
		if (minFlash == 0 || (flashOn == 1 && minFlash == 1)){
			GPIO_SetBits(GPIOB, GPIO_Pin_10);
		}
		break;

	case 4: //colon

		number = 10; //Use this to represent the two colon dots
		GPIO_SetBits(GPIOB, GPIO_Pin_14);
		break;

	case 5: //extra dot

		number = 11; //Use this to represent the am pm light

		if (amPmFlash == 0 || (flashOn == 1 && amPmFlash == 1)){

			if(amPm.RTC_H12 == RTC_H12_PM && MODEBUTTON == 0){
				GPIO_SetBits(GPIOB, GPIO_Pin_14);
			} else {
				GPIO_ResetBits(GPIOB, GPIO_Pin_14);
			}
		}
		break;

	}

//number = buttonPressed;
	switch (number){
	case 0:
		GPIO_SetBits(GPIOB, GPIO_Pin_4);
		GPIO_SetBits(GPIOB, GPIO_Pin_7);
		GPIO_SetBits(GPIOA, GPIO_Pin_8);
		GPIO_SetBits(GPIOE, GPIO_Pin_11);
		GPIO_SetBits(GPIOD, GPIO_Pin_3);
		GPIO_SetBits(GPIOE, GPIO_Pin_8);
		break;

	case 1:
		GPIO_SetBits(GPIOD, GPIO_Pin_3);
		GPIO_SetBits(GPIOB, GPIO_Pin_7);
		break;

	case 2:
		GPIO_SetBits(GPIOB, GPIO_Pin_4);
		GPIO_SetBits(GPIOE, GPIO_Pin_11);
		GPIO_SetBits(GPIOD, GPIO_Pin_3);
		GPIO_SetBits(GPIOD, GPIO_Pin_7);
		GPIO_SetBits(GPIOE, GPIO_Pin_8);
		break;

	case 3:
		GPIO_SetBits(GPIOB, GPIO_Pin_4);
		GPIO_SetBits(GPIOB, GPIO_Pin_7);
		GPIO_SetBits(GPIOD, GPIO_Pin_3);
		GPIO_SetBits(GPIOD, GPIO_Pin_7);
		GPIO_SetBits(GPIOE, GPIO_Pin_11);
		break;

	case 4:
		GPIO_SetBits(GPIOB, GPIO_Pin_7);
		GPIO_SetBits(GPIOA, GPIO_Pin_8);
		GPIO_SetBits(GPIOD, GPIO_Pin_3);
		GPIO_SetBits(GPIOD, GPIO_Pin_7);
		break;

	case 5:
		GPIO_SetBits(GPIOB, GPIO_Pin_4);
		GPIO_SetBits(GPIOB, GPIO_Pin_7);
		GPIO_SetBits(GPIOA, GPIO_Pin_8);
		GPIO_SetBits(GPIOE, GPIO_Pin_11);
		GPIO_SetBits(GPIOD, GPIO_Pin_7);
		break;

	case 6:
		GPIO_SetBits(GPIOB, GPIO_Pin_4);
		GPIO_SetBits(GPIOB, GPIO_Pin_7);
		GPIO_SetBits(GPIOA, GPIO_Pin_8);
		GPIO_SetBits(GPIOE, GPIO_Pin_11);
		GPIO_SetBits(GPIOD, GPIO_Pin_7);
		GPIO_SetBits(GPIOE, GPIO_Pin_8);
		break;

	case 7:
		GPIO_SetBits(GPIOB, GPIO_Pin_4);
		GPIO_SetBits(GPIOB, GPIO_Pin_7);
		GPIO_SetBits(GPIOD, GPIO_Pin_3);
		break;

	case 8:
		GPIO_SetBits(GPIOB, GPIO_Pin_4);
		GPIO_SetBits(GPIOB, GPIO_Pin_7);
		GPIO_SetBits(GPIOA, GPIO_Pin_8);
		GPIO_SetBits(GPIOE, GPIO_Pin_11);
		GPIO_SetBits(GPIOD, GPIO_Pin_3);
		GPIO_SetBits(GPIOD, GPIO_Pin_7);
		GPIO_SetBits(GPIOE, GPIO_Pin_8);
		break;

	case 9:
		GPIO_SetBits(GPIOB, GPIO_Pin_4);
		GPIO_SetBits(GPIOB, GPIO_Pin_7);
		GPIO_SetBits(GPIOA, GPIO_Pin_8);
		GPIO_SetBits(GPIOE, GPIO_Pin_11);
		GPIO_SetBits(GPIOD, GPIO_Pin_3);
		GPIO_SetBits(GPIOD, GPIO_Pin_7);
		break;

	case 10: //colon
		//Need to insert the anode pins for the colon (same as segments A & B)
		GPIO_SetBits(GPIOB, GPIO_Pin_4);
		GPIO_SetBits(GPIOD, GPIO_Pin_3);
		break;

	case 11: // dot
		//Need to insert the anode pins for the colon (same as segments A & B)
		GPIO_SetBits(GPIOB, GPIO_Pin_7);
		break;

	}
	//  }// end the "if" skip statement

}

uint8_t bcd(uint8_t dec) {
	//Divide by 10, bitwise shift left 4 then add modulus of division.
	//Returns 8 bit integer of proper BCD format
	// 0000 | 0000
	return (( dec / 10) << 4 ) + ( dec % 10);
}

uint8_t decimal(uint8_t bcd) {
	//
	return ((bcd >> 4) * 10)+ bcd % 16;
}

//used to increment time values
void setTime(void)
{


    // You can set the time with calls to the following function:
    // RTC_SetTime(RTC_Format_BCD, &myclockTimeStruct);
    // Read manuals for format and entries.

    //first time button pressed, set temp variable to real time
    if (tempTimeSet == 0) {
        tempSetTime.RTC_Hours = myclockTimeStruct.RTC_Hours;
        tempSetTime.RTC_Minutes = myclockTimeStruct.RTC_Minutes;
        tempSetTime.RTC_Seconds = myclockTimeStruct.RTC_Seconds;
        tempSetTime.RTC_H12 = myclockTimeStruct.RTC_H12;
        tempTimeSet++;
    }



    if( buttonPressed == 1 && TIMEUPBUTTON == 1){
         addHour(&tempSetTime);
         TIMEUPBUTTON = 0; //Turn off this button
    } else if (buttonPressed == 1) {
    	hoursFlash = 1;
    }

    //
    if (buttonPressed == 2 && TIMEUPBUTTON == 1){
         addMin(&tempSetTime);
         TIMEUPBUTTON = 0; //Turn off this button
     } else if (buttonPressed == 2) {
    	 hoursFlash = 0;
    	 minFlash = 1;
     }

    if (buttonPressed == 3){

         minFlash = 0;
         //reset temp time initializer
         tempTimeSet = 0;

         //Make temp time the real time
         myclockTimeStruct.RTC_Hours = tempSetTime.RTC_Hours;
         myclockTimeStruct.RTC_Minutes = tempSetTime.RTC_Minutes;
         myclockTimeStruct.RTC_Seconds = tempSetTime.RTC_Seconds;
         myclockTimeStruct.RTC_H12 = tempSetTime.RTC_H12;
         RTC_SetTime(RTC_Format_BCD, &myclockTimeStruct);

         //Set the button press count back to zero and turn off the "set time" button
         TIMEBUTTON = 0;
         buttonPressed = 0;

    }
}

//Adds minutes to time-holding variable passed in of type "RTC_TimeTypeDef"
//  When minutes reach 59, goes back to zero and does NOT add hour


void addMin(RTC_TimeTypeDef * timeStruct)
{
	//temporary variable to hold decimal minutes
	uint8_t temp_min;

	//convert BCD to decimal
	temp_min = decimal(timeStruct->RTC_Minutes);
	//increase min by one

	temp_min++;
	// AlarmStruct.RTC_AlarmTime.RTC_Minutes
	if (temp_min >= 60)
	{
		temp_min %= 60;
	}

	//convert back to BCD and spit into temp time variable
	timeStruct->RTC_Minutes = bcd(temp_min);

}

//Adds hours to time-holding variable passed in of type "RTC_TimeTypeDef"
void addHour(RTC_TimeTypeDef * timeStruct)
{
	//temporary variable to hold decimal minutes
	uint8_t temp_hours;

	//convert BCD to decimal
	temp_hours = decimal(timeStruct->RTC_Hours);
	//increase min by one

	temp_hours++;
	// AlarmStruct.RTC_AlarmTime.RTC_Minutes

	if (temp_hours > 12)
	{
		temp_hours = 1;
	} else if (temp_hours == 12) {
		amPmToggle(timeStruct);
	}

	//convert back to BCD and spit into temp time variable
	timeStruct->RTC_Hours = bcd(temp_hours);

}

//used to increment alarm values
void setAlarm(void)
{

	    // You can set the time with calls to the following function:
	    // RTC_SetTime(RTC_Format_BCD, &myclockTimeStruct);
	    // Read manuals for format and entries.

	    //first time button pressed, set temp variable to real time
	    if (tempAlarmSet == 0) {
	        tempSetTime.RTC_Hours = AlarmStruct.RTC_AlarmTime.RTC_Hours;
	        tempSetTime.RTC_Minutes = AlarmStruct.RTC_AlarmTime.RTC_Minutes;
	        tempSetTime.RTC_Seconds = AlarmStruct.RTC_AlarmTime.RTC_Seconds;
	        tempSetTime.RTC_H12 = AlarmStruct.RTC_AlarmTime.RTC_H12;
	        tempAlarmSet++;
	    }

	    if( alarmButtonPressed == 1 && TIMEUPBUTTON == 1){
	        addHour(&tempSetTime);
	        TIMEUPBUTTON = 0; //Turn off this button
	    } else if (alarmButtonPressed == 1) {
	    	hoursFlash = 1;
	    }

	    //
	    if (alarmButtonPressed == 2 && TIMEUPBUTTON == 1){
	         addMin(&tempSetTime);
	         TIMEUPBUTTON = 0; //Turn off this button
	     } else if (alarmButtonPressed == 2) {
	    	 hoursFlash = 0;
	    	 minFlash = 1;
	     }

	    if (alarmButtonPressed == 3) {

	         minFlash = 0;
	         //reset temp alarm initializer
	         tempAlarmSet = 0;

	         //disable alarm
	         RTC_AlarmCmd(RTC_Alarm_A,DISABLE);

	         //Make temp time the alarm time
	         AlarmStruct.RTC_AlarmTime.RTC_Hours = tempSetTime.RTC_Hours;
	         AlarmStruct.RTC_AlarmTime.RTC_Minutes = tempSetTime.RTC_Minutes;
	         AlarmStruct.RTC_AlarmTime.RTC_Seconds = tempSetTime.RTC_Seconds;
	         AlarmStruct.RTC_AlarmTime.RTC_H12 = tempSetTime.RTC_H12;

	         //Hard coding for testing
	         //AlarmStruct.RTC_AlarmTime.RTC_Hours = bcd(12);
	         //AlarmStruct.RTC_AlarmTime.RTC_Minutes = bcd(01);
	         //AlarmStruct.RTC_AlarmTime.RTC_Seconds = tempSetTime.RTC_Seconds;

	        //Then, pass alarm struct to RTC_SetAlarm
	         RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &AlarmStruct);

	         //Re-activate the alarm
	         RTC_AlarmCmd(RTC_Alarm_A,ENABLE);

	         //Clear the alarm flag
	         // RTC_ClearFlag(RTC_FLAG_ALRAF);

	         //Set the button press count back to zero and turn off the "set time" button
	         ALARMBUTTON = 0;
	         alarmButtonPressed = 0;

	    }
}



void amPmToggle(RTC_TimeTypeDef * timeStruct)
{
// am/pm toggle
	if (timeStruct->RTC_H12 == RTC_H12_AM){
		timeStruct->RTC_H12 = RTC_H12_PM;
	} else {
		timeStruct->RTC_H12 = RTC_H12_AM;
	}
}



//adds 10 minutes to the current time and checks to make sure that time is
//acceptable for a 12 hour clock.  It also stores the time when the button is
//first pressed so when the alarm is cancelled the alarm time goes back to
//the original
void snooze(void)
{
	//temporary variable to hold decimal minutes and hours
	uint8_t temp_min;
	uint8_t temp_hours;
	int hoursIncreased = 0;

	// add the original alarm time to memory
	if (1 == snoozeButtonPressed) {
		alarmMemory.RTC_AlarmTime = AlarmStruct.RTC_AlarmTime;
	}

	//convert BCD to decimal
	temp_min = decimal(AlarmStruct.RTC_AlarmTime.RTC_Minutes);
	temp_hours = decimal(AlarmStruct.RTC_AlarmTime.RTC_Hours);

	//increase min by ten
	temp_min+= 1;

	//Make sure that minutes don't exceed 60
	if (temp_min > 59)
	{
		temp_hours += 1;
		temp_min %= 60;
		hoursIncreased = 1;
	}

	if (temp_hours > 12) {
		temp_hours = 1;

	}

	if (hoursIncreased == 1 && temp_hours == 12){
		amPmToggle(&AlarmStruct.RTC_AlarmTime);
	}

	//Disaable alarm
    RTC_AlarmCmd(RTC_Alarm_A,DISABLE);

	//convert back to BCD and spit into alarm time variable variable
	AlarmStruct.RTC_AlarmTime.RTC_Minutes = bcd(temp_min);
	AlarmStruct.RTC_AlarmTime.RTC_Hours = bcd(temp_hours);

   //Then, pass alarm struct to RTC_SetAlarm
    RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &AlarmStruct);

    //Re-activate the alarm
    RTC_AlarmCmd(RTC_Alarm_A,ENABLE);

    SNOOZEBUTTON = 0;
}

//called by the timer interrupt to separate the BCD time values
void getCurrentTime(void)
{

	// Obtain current time.
	RTC_GetTime(RTC_Format_BCD, &myclockTimeStruct);

}


//converts the 12 hour time to 24 hour when displaying the time
//or alarm values
int convert24Hour(int hours, RTC_TimeTypeDef amPm)
{
	// If the display time is PM other than 12PM, add 12 to the hour.
	if (amPm.RTC_H12 == RTC_H12_PM && hours < 12) {
		hours += 12;
	} else if (hours == 12 && amPm.RTC_H12 == RTC_H12_AM){
		hours = 0;
	}

	return hours;
}

//called every timer interrupt.  checks for changes on the
//input data register and then debounces and sets which button is pressed
void buttonControls(void)
{

	//static uint16_t oldState = NOBUTTON;
	static uint16_t oldState = 0;
	static uint16_t newState = 0;

	//gets the current register value

	newState = (GPIOC->IDR) & NOBUTTON;

	//compares it to the previous register value
	//if they are different, reset the debug count
	if(oldState != newState )
	{
		debouncing = 0;
	}

	//otherwise increment the count
	else{

		debouncing++;
	}

    oldState = newState;

	//Pins used for buttons are C1,C2,C5,C6,C8,C9

	if (debouncing > 20) { //MAY HAVE TO INCREASE OR DECREASE THIS VALUE UPON TESTING

		//We have now reached a "stable" reading of a button press. Now, need to check which button is pressed


		if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) == 0 && buttonFlagC6 == 0){ //Set time

			TIMEBUTTON = 1;
			buttonFlagC6 = 1;

			//Set all other flags to zero
			TIMEUPBUTTON  = 0;
			ALARMBUTTON = 0;
			SNOOZEBUTTON = 0;
			RESETBUTTON = 0;

			//  TimeSet function needs to know if the timeset button has been pressed
			//  once (set hour), twice (set min), or three times (am/pm) and 4 times (done setting time)
			buttonPressed++;
			alarmButtonPressed = 0;
			tempAlarmSet = 0;
			if (buttonPressed > 3 ) buttonPressed = 0;

		} else if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1) == 0 && buttonFlagC1 == 0){ //Set alarm

			ALARMBUTTON = 1;
			buttonFlagC1 = 1;

			//Set all other flags to zero
			TIMEUPBUTTON  = 0;
			TIMEBUTTON = 0;
			SNOOZEBUTTON = 0;
			RESETBUTTON = 0;

			//  AlarmSet function needs to know if the alarm set button has been pressed
			//  once (set hour), twice (set min), or three times (done setting time)
			alarmButtonPressed++;
			buttonPressed = 0;
			tempTimeSet = 0;
			if (alarmButtonPressed > 3 ) alarmButtonPressed = 0;

		} else if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2) == 0 && buttonFlagC2 == 0){  //Time "up"
			buttonFlagC2 = 1;
			TIMEUPBUTTON = 1;

			//Set all other flags to zero
			SNOOZEBUTTON = 0;
			RESETBUTTON = 0;

		} else if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) == 0 && buttonFlagC5 == 0){  //Snooze
			buttonFlagC5 = 1;
			SNOOZEBUTTON = 1;

			//Set all other flags to zero
			TIMEUPBUTTON  = 0;
			TIMEBUTTON = 0;
			ALARMBUTTON = 0;
			RESETBUTTON = 0;

		} else if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9) == 0 && buttonFlagC9 == 0){   //Alarm "off"
			buttonFlagC9 = 1;
			RESETBUTTON = 1;

			TIMEUPBUTTON  = 0;
			TIMEBUTTON = 0;
			ALARMBUTTON = 0;
			SNOOZEBUTTON = 0;

			buttonPressed = 0;
			alarmButtonPressed = 0;
			tempTimeSet = 0;
			tempAlarmSet = 0;

		}

		//Mode Button
		if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8) == 0){   //24 hour mode
			MODEBUTTON = 1;
		} else {
			MODEBUTTON = 0;
		}


		//Need to reset flags for each button
		if ( (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) == 1 && buttonFlagC6 == 1)) {
			buttonFlagC6 = 0;
		}
		if ( (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1) == 1 && buttonFlagC1 == 1)) {
			buttonFlagC1 = 0;
		}
		if ( (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2) == 1 && buttonFlagC2 == 1)) {
			buttonFlagC2 = 0;
		}
		if ( (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5) == 1 && buttonFlagC5 == 1)) {
			buttonFlagC5 = 0;
		}
		if ( (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9) == 1 && buttonFlagC9 == 1)) {
			buttonFlagC9 = 0;
		}


	}
}


//configures the clocks, gpio, alarm, interrupts etc.
void configuration(void)
{

	//lets the system clocks be viewed
	RCC_GetClocksFreq(&RCC_Clocks);

	//enable peripheral clocks
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);    // Needed for Audio chip

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	//enable the RTC
	PWR_BackupAccessCmd(DISABLE);
	PWR_BackupAccessCmd(ENABLE);
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	RCC_RTCCLKCmd(ENABLE);
	RTC_AlarmCmd(RTC_Alarm_A,DISABLE);

	//Enable the LSI OSC
	RCC_LSICmd(ENABLE);

	//Wait till LSI is ready
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

	//enable the external interrupt for the RTC to use the Alarm
	// EXTI configuration
	EXTI_ClearITPendingBit(EXTI_Line17);
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	//set timer 5 to interrupt at a rate of 500Hz
	TIM_TimeBaseStructInit(&TIM_InitStruct);
	TIM_InitStruct.TIM_Period   =  8000;    // 500Hz
	TIM_InitStruct.TIM_Prescaler = 20;
	TIM_TimeBaseInit(TIM5, &TIM_InitStruct);

	// Enable the TIM5 global Interrupt
	NVIC_Init( &NVIC_InitStructure );
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStructure );

	//setup the RTC for 12 hour format
	myclockInitTypeStruct.RTC_HourFormat = RTC_HourFormat_12;
	myclockInitTypeStruct.RTC_AsynchPrediv = 127;
	myclockInitTypeStruct.RTC_SynchPrediv = 0x00FF;
	RTC_Init(&myclockInitTypeStruct);

	//set the time displayed on power up to 12PM
	myclockTimeStruct.RTC_H12 = RTC_H12_PM;
	//myclockTimeStruct.RTC_Hours = 0x012;
	//myclockTimeStruct.RTC_Minutes =  0x00;
	myclockTimeStruct.RTC_Hours =bcd(11);
	myclockTimeStruct.RTC_Minutes =  bcd(58);
	myclockTimeStruct.RTC_Seconds =  0x00;
	RTC_SetTime(RTC_Format_BCD, &myclockTimeStruct);


	//sets alarmA for 12:00AM, date doesn't matter
	AlarmStruct.RTC_AlarmTime.RTC_H12 = RTC_H12_PM;
	//AlarmStruct.RTC_AlarmTime.RTC_Hours = 0x07;
	//AlarmStruct.RTC_AlarmTime.RTC_Minutes = 0x00;
	AlarmStruct.RTC_AlarmTime.RTC_Hours = bcd(11);
	AlarmStruct.RTC_AlarmTime.RTC_Minutes = bcd(58);
	AlarmStruct.RTC_AlarmTime.RTC_Seconds =0x00;
	AlarmStruct.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;
	RTC_SetAlarm(RTC_Format_BCD,RTC_Alarm_A,&AlarmStruct);

	// Enable the Alarm global Interrupt
	NVIC_Init( &NVIC_InitStructure );
	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStructure );

	// Pins B6 and B9 are used for I2C communication, configure as alternate function.
	// We use them to communicate with the CS43L22 chip via I2C
	GPIO_StructInit( &GPIOInitStruct );
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_9; // we are going to use PB6 and PB9
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_AF;            // set pins to alternate function
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_50MHz;       // set GPIO speed
	GPIOInitStruct.GPIO_OType = GPIO_OType_OD;          // set output to open drain --> the line has to be only pulled low, not driven high
	GPIOInitStruct.GPIO_PuPd = GPIO_PuPd_UP;            // enable pull up resistors
	GPIO_Init(GPIOB, &GPIOInitStruct);

	// The I2C1_SCL and I2C1_SDA pins are now connected to their AF
	// so that the I2C1 can take over control of the
	//  pins
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);

	// Set the I2C structure parameters
	// I2C is used to configure CS43L22 chip for analog processing on the headphone jack.
	I2C_InitStruct.I2C_ClockSpeed = 100000;             // 100kHz
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;         // I2C mode
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2; // 50% duty cycle --> standard
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;          // own address, not relevant in master mode
	I2C_InitStruct.I2C_Ack = I2C_Ack_Disable;           // disable acknowledge when reading (can be changed later on)
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // set address length to 7 bit addresses

	// Initialize the I2C peripheral w/ selected parameters
	I2C_Init(I2C1,&I2C_InitStruct);

	// enable I2C1
	I2C_Cmd(I2C1, ENABLE);

	//IO for push buttons using internal pull-up resistors
	GPIO_StructInit( &GPIOInitStruct );
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIOInitStruct.GPIO_OType = GPIO_OType_PP;
	GPIOInitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOC, &GPIOInitStruct);

	//configure GPIO pin 11 for motor
	GPIO_StructInit( &GPIOInitStruct );
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIOInitStruct.GPIO_OType = GPIO_OType_PP;
	GPIOInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIOInitStruct);
	//STart with it off
	GPIO_ResetBits(GPIOC, GPIO_Pin_11);



	//configure GPIO for LED
	GPIO_StructInit( &GPIOInitStruct );
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_11;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIOInitStruct.GPIO_OType = GPIO_OType_PP;
	GPIOInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIOInitStruct);

	GPIO_StructInit( &GPIOInitStruct );
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_7 | GPIO_Pin_10 | GPIO_Pin_14;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIOInitStruct.GPIO_OType = GPIO_OType_PP;
	GPIOInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIOInitStruct);

	GPIO_StructInit( &GPIOInitStruct );
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_7;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIOInitStruct.GPIO_OType = GPIO_OType_PP;
	GPIOInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIOInitStruct);

	GPIO_StructInit( &GPIOInitStruct );
	GPIOInitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10;
	GPIOInitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIOInitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIOInitStruct.GPIO_OType = GPIO_OType_PP;
	GPIOInitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIOInitStruct);


	//enables RTC alarm A interrupt
	RTC_ITConfig(RTC_IT_ALRA, ENABLE);

	//enables timer interrupt
	TIM5->DIER |= TIM_IT_Update;

	//enables timer
	TIM5->CR1 |= TIM_CR1_CEN;

}
