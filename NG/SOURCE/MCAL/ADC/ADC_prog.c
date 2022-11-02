/*
 * ADC_cprog.c
 *
 *  Created on: Nov 1, 2022
 *      Author: Manuel , Shrouk , Khaled , Nadim
 */
#include "../../LIBRARY/stdTypes.h"
#include "../../LIBRARY/errorState.h"

#include "ADC_config.h"
#include "ADC_priv.h"


ES_t ADC_enuInit(void)
{
	ES_t Local_enuErrorState = ES_NOK ;
#if ADC_REF<= 3 && ADC_ADJ <=1 && ADC_INIT_CHNL <=31
	ADMUX = 0;
	ADMUX |= (ADC_REF<<6);
	ADMUX |= (ADC_ADJ<<5);
	ADMUX |= (ADC_INIT_CHNL);
#else
#error " initial values out of range "
#endif
	ADCSRA = 0;
#if ADC_MODE <=CAPTUREEVENT_T1
	ADCSRA |= (1<<5);
	SFIOR &= (0x1F);
	SFIOR |= (ADC_MODE <<5);
#elif ADC_MODE == SINGLE
	ADCSRA &= ~(1<<5);
#else
#error" initial values out of range"
#endif
#if ADC_INT_MODE <=1 && ADC_PRES <= 7
	ADCSRA |= (ADC_INT_MODE<<3);
	ADCSRA |= (ADC_PRES);
	Local_enuErrorState = ES_OK;
#else
#error " initial values out of range "
#endif
	ADCSRA |= (1<<7);			//enable ADC
	return  Local_enuErrorState ;
}

ES_t ADC_enuStartConversion(void)
{
	ES_t Local_enuErrorState = ES_OK ;

	ADCSRA |= (1<<6);
	return  Local_enuErrorState ;
}

ES_t ADC_enuRead(u16 * Copy_pu16Value)
{
	ES_t Local_enuErrorState = ES_NOK ;

	if (Copy_pu16Value==NULL)	return ES_NULL_POINTER;
	while(!(((ADCSRA)>>4)&1));
	if(((ADMUX>>5)&1)==LEFT_ADJ)	* Copy_pu16Value = (ADCL>>6)|((u16)ADCH<<2);
	else * Copy_pu16Value = ADCL | ((u16)ADCH<<8);
	Local_enuErrorState = ES_OK ;
	return  Local_enuErrorState ;
}

ES_t ADC_enuPollingRead(u16 * Copy_pu16ADCValue)
{
	ES_t Local_enuErrorState = ES_NOK ;
	if (Copy_pu16ADCValue==NULL)	return ES_NULL_POINTER;
	while(!((ADCSRA>>4)&1));
	if(((ADMUX>>5)&1)==LEFT_ADJ)
	{

		* Copy_pu16ADCValue = (ADCL>>6)|((u16)ADCH<<2);
	}
	else
	{
		* Copy_pu16ADCValue = ADCL | ((u16)ADCH<<8);
	}
	ADCSRA |= (1<<4);
	Local_enuErrorState = ES_OK ;

	return  Local_enuErrorState ;
}

