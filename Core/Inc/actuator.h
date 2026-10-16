/**
 * @file actuator.h
 * @brief Filter actuator motion control interface.
 *
 * Homing and absolute-position moves for the four-position filter actuator.
 * STEP pulses for the MP6602 driver come from TIM3 channel 3 PWM and the
 * direction is set with the DIR pin. Pulses are counted in
 * HAL_TIM_PeriodElapsedCallback() (main.c), which ends each move.
 *
 * @author james
 * @date Oct 6, 2026
 */

#ifndef INC_ACTUATOR_H_
#define INC_ACTUATOR_H_

#include "main.h"
#include "stdint.h"
#include "stdio.h"
#include "stdbool.h"
#include "led.h"

extern TIM_HandleTypeDef htim3;	///< TIM3 handle (defined in main.c); CH3 PWM generates the STEP pulses.
/* Shared actuator state, defined and documented in main.c. */
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

/**
 * @brief Home the actuator on the limit switch, then move to position 1.
 *
 * If the limit switch (LMT_SW, active low) is not already pressed, the
 * actuator is driven toward it (DIR = 1) until it closes. After a short
 * software delay the direction is reversed and an interrupt-driven move of
 * ::gl_actuator_offset pulses is started to reach filter position 1.
 *
 * @note Busy-waits while seeking the limit switch. The final move to
 *       position 1 may still be in progress on return; wait for
 *       ::gl_inmotion to clear.
 * @post ::gl_homed is true, ::gl_filter_position is 1 and the position LED
 *       is updated.
 */
void actuator_home(void);

/**
 * @brief Start a move to an absolute filter position.
 *
 * Computes the pulse count between the current position and @p pos from
 * ::gl_pos_counts, sets the DIR pin and starts TIM3 PWM with update
 * interrupts. The move finishes asynchronously: ::gl_inmotion is cleared by
 * HAL_TIM_PeriodElapsedCallback() when the pulse count is reached.
 *
 * The request is ignored if the actuator is already at @p pos or a move is
 * in progress.
 *
 * @param pos Target filter position, 1 to 4.
 * @pre  The actuator has been homed with actuator_home().
 * @warning @p pos is not range-checked here; callers must pass 1 to 4.
 */
void actuator_move_pos(uint8_t pos);

#endif /* INC_ACTUATOR_H_ */
