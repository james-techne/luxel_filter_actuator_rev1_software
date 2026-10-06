/*
 * actuator.h
 *
 *  Created on: Oct 6, 2026
 *      Author: james
 */

#ifndef INC_ACTUATOR_H_
#define INC_ACTUATOR_H_

#include "main.h"
#include "stdint.h"
#include "stdio.h"
#include "stdbool.h"
#include "led.h"

extern TIM_HandleTypeDef htim3;
extern bool gl_inmotion;
extern bool gl_homed;
extern bool gl_limit;
extern uint32_t gl_pulseperposition;
extern uint8_t gl_filter_position;
extern uint32_t gl_actuator_position;
extern uint32_t gl_actuator_offset;
extern uint32_t gl_positiondelta;
extern uint32_t gl_pos_counts[4];
extern uint32_t gl_pulse_cnt;
void actuator_home(void);
void actuator_move_pos(uint8_t pos);

#endif /* INC_ACTUATOR_H_ */
