
#include "stm32f4xx_rtc.h"

#ifndef MAIN_H_
#define MAIN_H_




#define NOBUTTON 0b0000001101100110
//#define NOBUTTON 0xB30

//RTC structures
RTC_InitTypeDef		myclockInitTypeStruct;
RTC_TimeTypeDef		myclockTimeStruct;
RTC_AlarmTypeDef	AlarmStruct;
RTC_AlarmTypeDef 	alarmMemory;


#endif /* MAIN_H_ */
