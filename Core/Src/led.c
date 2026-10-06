/*
 * led.c
 *
 *  Created on: Oct 4, 2026
 *      Author: james
 */

#include "../Inc/led.h"

void set_led_pos(uint8_t pos)
{
	//POS1 and POS4 are on GPIOB, POS2 and POS3 are on GPIOA
	switch(pos){
	case	1	:	HAL_GPIO_WritePin(GPIOB, POS1_LED_Pin, RESET); HAL_GPIO_WritePin(GPIOB, POS4_LED_Pin, SET); HAL_GPIO_WritePin(GPIOA, POS2_LED_Pin | POS3_LED_Pin, SET); break;
	case	2	:	HAL_GPIO_WritePin(GPIOA, POS2_LED_Pin, RESET); HAL_GPIO_WritePin(GPIOA, POS3_LED_Pin, SET); HAL_GPIO_WritePin(GPIOB, POS1_LED_Pin | POS4_LED_Pin, SET); break;
	case	3	: 	HAL_GPIO_WritePin(GPIOA, POS3_LED_Pin, RESET); HAL_GPIO_WritePin(GPIOA, POS2_LED_Pin, SET); HAL_GPIO_WritePin(GPIOB, POS1_LED_Pin | POS4_LED_Pin, SET); break;
	case	4	:	HAL_GPIO_WritePin(GPIOB, POS4_LED_Pin, RESET); HAL_GPIO_WritePin(GPIOA, POS1_LED_Pin, SET); HAL_GPIO_WritePin(GPIOA, POS2_LED_Pin | POS3_LED_Pin, SET); break;
	default		:	break;
	}
} //set_led_os
