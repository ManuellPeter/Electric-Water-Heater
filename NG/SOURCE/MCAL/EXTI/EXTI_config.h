/*
 * EXTI_config.h
 *
 *  Created on: Nov 1, 2022
 *      Author: Manuel , Shrouk , Khaled , Nadim
 */

#ifndef MCAL_EXTI_EXTI_CONFIG_H_
#define MCAL_EXTI_EXTI_CONFIG_H_

#define EXTI_NUM		3

typedef struct
{
	u8 intPin;
	u8 IntMode;
	u8 senseLvl;
}EXTI_t;

#endif /* MCAL_EXTI_EXTI_CONFIG_H_ */
