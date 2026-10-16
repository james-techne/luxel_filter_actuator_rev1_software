/**
 * @file led.h
 * @brief Filter position indicator LEDs.
 *
 * @author james
 * @date Oct 4, 2026
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include "stm32f0xx_hal.h"
#include <stdint.h>
#include <main.h>

/**
 * @brief Light the LED for a filter position and turn the others off.
 *
 * The LEDs are active low (a pin driven low lights its LED). POS1 and POS4
 * LEDs are on GPIOB; POS2 and POS3 LEDs are on GPIOA.
 * @param pos Filter position, 1 to 4. Other values leave the LEDs unchanged.
 */
void set_led_pos(uint8_t pos);

#endif /* INC_LED_H_ */
