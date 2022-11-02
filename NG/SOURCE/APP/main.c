/*
 *
 *  Created on: Nov 1, 2022
 *      Author: Manuel , Shrouk , Khaled , Nadim
 */

#include "../LIBRARY/stdTypes.h"
#include "../LIBRARY/errorState.h"
#include "../MCAL/DIO/DIO_int.h"
#include "../MCAL/GIE/GIE_int.h"
#include "../HAL/two_7seg_decoder/two_7SEG_Int.h"
#include "../MCAL/EXTI/EXTI_config.h"
#include "../MCAL/EXTI/EXTI_int.h"
#include "../MCAL/TIMER2/TIMER2_int.h"
#include "../HAL/Temp_sensor/Temp_sensor_intr.h"

#define TempretureADD		0

/* Pins Configurations*/
#define UP_Button			DIO_u8GROUP_D,DIO_u8PIN2 // For Increasing Temp
#define DOWN_Button			DIO_u8GROUP_D,DIO_u8PIN3 // For Decreasing Temp
#define ONOFF_Button		DIO_u8GROUP_B,DIO_u8PIN2 // Starts and Stops the System
#define Heater_Pin			DIO_u8GROUP_A,DIO_u8PIN1 // Heater Relay Pin
#define HeaterLed_Pin		DIO_u8GROUP_B,DIO_u8PIN1
#define Coolant_Pin			DIO_u8GROUP_B,DIO_u8PIN0 // Cooler Relay Pin
#define CoolantLed_Pin		DIO_u8GROUP_B,DIO_u8PIN3
/**********************/

#define	TempSetTime			5000 //ms
#define TempReadperiod		100
#define TOP					256
#define PRESCALLER			1024
#define OVFSTempSetCounts	((u64)TempSetTime * (F_CPU/1000)/((u64)PRESCALLER*TOP))
#define TempReadCounts		((u64)TempReadperiod * (F_CPU/1000)/((u64)PRESCALLER*TOP))
#define One_secound			((u64)1000 * (F_CPU/1000)/((u64)PRESCALLER*TOP))

u8 OneSecCounts = One_secound;
u16 TempSetCounter = 0;
u16 GlobalCounter=0;
extern EXTI_t EXTI_AstrEXTIConfig [];
u8 temp_set = 60;

typedef enum{ON,OFF}Power; // Power modes

Power Controller_PowerMood=OFF;

void EXTI_ONOFFfun() // Interrupt Function to Start/Stop the system
{
	if(Controller_PowerMood == OFF)
	{
		EXTI_enuEnableInterrupt(&EXTI_AstrEXTIConfig[0]);
		EXTI_enuEnableInterrupt(&EXTI_AstrEXTIConfig[1]);
		TIMER2_enuEnableInterrupt(TIM2_OVERFLOW_INTER);
		Controller_PowerMood = ON;
	}
	else
	{
		EXTI_enuDisableInterrupt(&EXTI_AstrEXTIConfig[0]);
		EXTI_enuDisableInterrupt(&EXTI_AstrEXTIConfig[1]);
		TIMER2_enuDisableInterrupt(TIM2_OVERFLOW_INTER);
		Controller_PowerMood = OFF;
		S7SEG2_enuSendInteger(0);
		DIO_enuSetPinValue(CoolantLed_Pin,DIO_u8LOW);
		DIO_enuSetPinValue(Coolant_Pin,DIO_u8LOW);
		DIO_enuSetPinValue(HeaterLed_Pin,DIO_u8LOW);
		DIO_enuSetPinValue(Heater_Pin,DIO_u8LOW);
	}
}

void IncreaseTemp() // Increaing Temp Function
{
	if(temp_set<75 && TempSetCounter)
	{
		temp_set += 5;
	}
	TempSetCounter = OVFSTempSetCounts;
}

void DecreaseTemp() //Decreasing Temp Function
{
	if(temp_set>35 && TempSetCounter)
	{
		temp_set -= 5;
	}
	TempSetCounter = OVFSTempSetCounts;
}

void Timerovfcounter()
{
	if(GlobalCounter)	GlobalCounter--;
	if(OneSecCounts)	OneSecCounts--;
	if(TempSetCounter)
	{
		TempSetCounter--;
		if(!TempSetCounter)
			EEPROM_write(TempretureADD,temp_set);
	}
}

int main(void)
{
	u16 Current_Temp;
	S7SEG2_enuInit();
	EXTI_enuInit();
	EXTI_enuCallBack(&EXTI_AstrEXTIConfig[0],IncreaseTemp,NULL);
	EXTI_enuCallBack(&EXTI_AstrEXTIConfig[1],DecreaseTemp,NULL);
	EXTI_enuCallBack(&EXTI_AstrEXTIConfig[2],EXTI_ONOFFfun,NULL);
	/* Pins Direction Configuration */
	DIO_enuSetPinDirection(UP_Button,DIO_u8INPUT);
	DIO_enuSetPinDirection(DOWN_Button,DIO_u8INPUT);
	DIO_enuSetPinDirection(ONOFF_Button,DIO_u8INPUT);

	DIO_enuSetPinDirection(Heater_Pin,DIO_u8OUTPUT);
	DIO_enuSetPinDirection(HeaterLed_Pin,DIO_u8OUTPUT);

	DIO_enuSetPinDirection(Coolant_Pin,DIO_u8OUTPUT);
	DIO_enuSetPinDirection(CoolantLed_Pin,DIO_u8OUTPUT);

	/* Pins Level Configuration */
	DIO_enuSetPinValue(UP_Button,DIO_u8HIGH);
	DIO_enuSetPinValue(DOWN_Button,DIO_u8HIGH);
	DIO_enuSetPinValue(ONOFF_Button,DIO_u8HIGH);

	DIO_enuSetPinValue(Heater_Pin,DIO_u8LOW);
	DIO_enuSetPinValue(HeaterLed_Pin,DIO_u8LOW);

	DIO_enuSetPinValue(CoolantLed_Pin,DIO_u8LOW);
	DIO_enuSetPinValue(Coolant_Pin,DIO_u8LOW);
	/***************************/

	TEMP_voidInit();
	TEMP_u16GetValue(&Current_Temp);
	GIE_enuEnable();
	TIMER2_enuInit();
	TIMER2_enuCallBackFun(TIM2_OVERFLOW_INTER,Timerovfcounter,NULL);
	TempSetCounter = 0;

	{
		u8 EpromRead;
		EEPROM_read(TempretureADD,&EpromRead);
		if(EpromRead!=0XFF)		temp_set=EpromRead;
	}

	u8 TempArray[10]={Current_Temp,Current_Temp,Current_Temp,Current_Temp,Current_Temp,Current_Temp,Current_Temp,Current_Temp,Current_Temp,Current_Temp};
	Controller_PowerMood=OFF;
	EXTI_enuDisableInterrupt(&EXTI_AstrEXTIConfig[0]);
	EXTI_enuDisableInterrupt(&EXTI_AstrEXTIConfig[1]);
	TIMER2_enuDisableInterrupt(TIM2_OVERFLOW_INTER);

	while(1)
	{
		while(Controller_PowerMood == OFF);

		if(TempSetCounter)		S7SEG2_enuSendInteger(temp_set);
		else			S7SEG2_enuSendInteger(Current_Temp);

		if(!GlobalCounter)
		{
			GlobalCounter=TempReadCounts;
			static u8 TempIndex=0;
			TEMP_u16GetValue(&Current_Temp);
			if (TempIndex == 10)
				TempIndex = 0;
			TempArray[TempIndex++] =  Current_Temp;
		}

		u16 TempAvarage=(TempArray[0]+TempArray[1]+TempArray[2]+TempArray[3]+TempArray[4]+TempArray[5]+TempArray[6]+TempArray[7]+TempArray[8]+TempArray[9])/10;
		u8 Coolerpinstate;
		DIO_enuGetPinValue(Coolant_Pin,&Coolerpinstate);
		u8 Heaterpinstate;
		DIO_enuGetPinValue(Heater_Pin,&Heaterpinstate);

		if(TempAvarage < (temp_set-5))
		{
			if(Heaterpinstate!=DIO_u8HIGH)
			{
				DIO_enuSetPinValue(Heater_Pin,DIO_u8HIGH);
				DIO_enuSetPinValue(Coolant_Pin,DIO_u8LOW);
				DIO_enuSetPinValue(CoolantLed_Pin,DIO_u8LOW);
			}
		}
		else if( TempAvarage > ( temp_set+5))
		{
			if(Coolerpinstate!=DIO_u8HIGH)
			{
				DIO_enuSetPinValue(Heater_Pin,DIO_u8LOW);
				DIO_enuSetPinValue(Coolant_Pin,DIO_u8HIGH);
				DIO_enuSetPinValue(HeaterLed_Pin,DIO_u8LOW);
			}
		}

		if(!(OneSecCounts)&& Coolerpinstate==DIO_u8HIGH)
		{
			OneSecCounts = One_secound;
			DIO_enuTogglePinValue(CoolantLed_Pin);
		}

		else if(!(OneSecCounts)&&Heaterpinstate==DIO_u8HIGH)
		{
			OneSecCounts = One_secound;
			DIO_enuTogglePinValue(HeaterLed_Pin);
		}
	}
}
