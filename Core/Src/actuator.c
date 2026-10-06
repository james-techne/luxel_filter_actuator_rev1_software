/*
 * actuator.c
 *
 *  Created on: Oct 6, 2026
 *      Author: james
 */

#include "../Inc/actuator.h"

void actuator_home(void)
{
	uint32_t ii;
	// If Limit switch is already depressed, move off limit
//	if(HAL_GPIO_ReadPin(GPIOB, LMT_SW_Pin) == GPIO_PIN_RESET)
//	{
//		gl_positiondelta = 8000;
//		gl_pulse_cnt = 0;
//		gl_inmotion = true;
//		HAL_GPIO_WritePin(GPIOB, DIR_Pin, RESET);
//		HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_3);
//		HAL_TIM_Base_Start_IT(&htim3);
//		while(gl_inmotion){};
//		for(ii=0; ii<0x7FFFFF; ii++){}
//		HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_3);
//		HAL_TIM_Base_Stop_IT(&htim3);
//
//	}

	// IF limit switch is not depressed, move to limit
	if(HAL_GPIO_ReadPin(GPIOB, LMT_SW_Pin) == GPIO_PIN_SET)
	{
		HAL_GPIO_WritePin(GPIOB, DIR_Pin, SET);
		gl_inmotion = true;
		HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
		HAL_TIM_Base_Start(&htim3);

		while(HAL_GPIO_ReadPin(GPIOB, LMT_SW_Pin) == GPIO_PIN_SET){};
		//__HAL_GPIO_EXTI_CLEAR_IT(LMT_SW_Pin);
		HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_3);
		HAL_TIM_Base_Stop(&htim3);
		gl_inmotion = false;
	}

	for(ii=0; ii<0x7FFFFF; ii++){}
	__HAL_GPIO_EXTI_CLEAR_IT(LMT_SW_Pin);

	gl_positiondelta = gl_actuator_offset;
	gl_pulse_cnt = 0;
	HAL_GPIO_WritePin(GPIOB, DIR_Pin, RESET);
	gl_inmotion = true;
	HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_3);
	HAL_TIM_Base_Start_IT(&htim3);

	gl_homed = true;



//	if(HAL_GPIO_ReadPin(GPIOB, LMT_SW_Pin) == GPIO_PIN_RESET)
//	{
//		gl_positiondelta = gl_actuator_offset;
//		HAL_GPIO_WritePin(GPIOB, DIR_Pin, RESET);
//		gl_inmotion = true;
//		HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_3);
//		HAL_TIM_Base_Start_IT(&htim3);
//	}

	gl_filter_position = 1;
	gl_pulse_cnt = 0;
	gl_actuator_position = gl_actuator_offset;
	set_led_pos(gl_filter_position);


} // actuator_home

void actuator_move_pos(uint8_t pos)
{
	if(gl_filter_position != pos)
	{
		//Determine direction
		if(pos > gl_filter_position)
		{
			HAL_GPIO_WritePin(GPIOB, DIR_Pin, RESET);
			//Determine pulse count
			gl_positiondelta = gl_pos_counts[pos-1] - gl_pos_counts[gl_filter_position -1];
		}
		else
		{
			HAL_GPIO_WritePin(GPIOB, DIR_Pin, SET);
			//Determine pulse count
			gl_positiondelta = gl_pos_counts[gl_filter_position -1] - gl_pos_counts[pos-1];
		}

		gl_inmotion = true;
		HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_3);
		HAL_TIM_Base_Start_IT(&htim3);

		gl_filter_position = pos;
		set_led_pos(gl_filter_position);

	}
} //actuator_move_pos
