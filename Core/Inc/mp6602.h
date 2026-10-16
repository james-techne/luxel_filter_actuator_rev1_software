/**
 * @file mp6602.h
 * @brief Interface for the MPS MP6602 stepper motor driver.
 *
 * The MP6602 is configured over SPI1 with a software chip select (SPI_CS on
 * GPIOA). Each 16-bit frame carries the register address and read/write bit
 * in its upper four bits and data in the lower twelve. STEP pulses come from
 * TIM3 CH3 and direction from the DIR pin (see actuator.h).
 *
 * @note The two bytes of a frame appear on the bus in the reverse of their
 *       order in the transmit array, so mp6602_init() stores a frame 0xHHLL
 *       as {0xLL, 0xHH}. See the SPI notes in main().
 *
 * @author james
 * @date Aug 19, 2026
 */

#ifndef INC_MP6602_H_
#define INC_MP6602_H_

#include <stdio.h>
#include <stdint.h>
#include "./main.h"

extern SPI_HandleTypeDef hspi1;	///< SPI1 handle (defined in main.c) connected to the MP6602.

// Maybe include an array of values for each register to initialize
// the chip on board start up



// Register Read/Write
/**
 * @name Register access codes
 * Upper nibble of the SPI frame: bits 15:13 select the register and bit 12
 * is set for a write.
 * @{
 */
#define CTRL_READ	0x0000	///< Read the CTRL register.
#define CTRL_WRITE	0x1000	///< Write the CTRL register.
#define CTRL2_READ	0x2000	///< Read the CTRL2 register.
#define CTRL2_WRITE	0x3000	///< Write the CTRL2 register.
#define ISET_READ	0x4000	///< Read the ISET register.
#define ISET_WRITE	0x5000	///< Write the ISET register.
#define STALL_READ	0x6000	///< Read the STALL register.
#define STALL_WRTE	0x7000	///< Write the STALL register.
#define BEMF_READ	0x8000	///< Read the BEMF register.
#define BEMF_WRITE	0x9000	///< Write the BEMF register.
#define TSTP_READ	0xA000	///< Read the TSTP register.
#define TSTP_WRITE	0xB000	///< Write the TSTP register.
#define OCP_READ	0xC000	///< Read the OCP register.
#define	OCP_WRITE	0xD000	///< Write the OCP register.
#define FAULT_READ	0xE000	///< Read the FAULT register.
#define FAULT_WRITE	0xF000	///< Write the FAULT register.
/** @} */

// CTRL Masks
/**
 * @name CTRL register fields
 * @{
 */
#define CTRL_EN		0x0001	///< Enable bit.
#define CTRL_STEP	0x0002	///< Step bit.
#define CTRL_DIR	0x0004	///< Direction bit.
// Microstepping (MS)
#define CTRL_MS_FULL	0x0038	///< Microstepping: full step.
#define CTRL_MS_HALF	0x0030	///< Microstepping: half step.
#define CTRL_MS_QUART	0x0008	///< Microstepping: 1/4 step.
#define CTRL_MS_8TH		0x0018	///< Microstepping: 1/8 step.
#define CTRL_MS_16TH	0x0020	///< Microstepping: 1/16 step.
#define CTRL_MS_32ND	0x0028	///< Microstepping: 1/32 step.
//Off time (OT)
#define CTRL_OT_20US	0x01C0	///< Off time: 20 us.
#define CTRL_OT_25US	0x0040	///< Off time: 25 us.
#define CTRL_OT_30US	0x0080	///< Off time: 30 us.
#define CTRL_OT_35US	0x00C0	///< Off time: 35 us.
#define CTRL_OT_40US	0x0100	///< Off time: 40 us.
#define CTRL_OT_45US	0x0140	///< Off time: 45 us.
#define CTRL_OT_50US	0x0180	///< Off time: 50 us.
#define CTRL_OT_55US	0x01C0	///< Off time: 55 us.
#define CTRL_AH_DIS		0x0E00	///< Automatic hold disabled.
// Fill in the rest of the Automatic Hold (AH)
/** @} */


// Function Propotypes
/**
 * @brief Read an MP6602 register.
 * @param reg Register index.
 * @return Register contents.
 * @note Declared but not yet implemented (the definition is commented out).
 */
uint16_t mp6602_read(uint8_t reg);

/**
 * @brief Write an MP6602 register.
 * @param reg Register index.
 * @param val Value to write.
 * @note Declared but not yet implemented (the definition is commented out).
 */
void mp6602_write(uint8_t reg, uint16_t val);

/**
 * @brief Configure the MP6602 for this application.
 *
 * Releases nRST, then writes all eight registers over SPI. The current
 * settings select 1/16 microstepping.
 */
void mp6602_init(void);

/**
 * @brief Write the CTRL register to set the driver enable bit.
 */
void mp6602_enable(void);

/**
 * @brief Write the reset values in ::mp6602_reset_vals back to the MP6602.
 * @note The loop currently sends only the first frame.
 */
void mp6602_reset(void);

/**
 * @brief Handle an MP6602 fault (nFAULT asserted).
 * @note Declared but not yet implemented.
 */
void mp6602_faulthandle(void);
void mp6602_init(void);








#endif /* INC_MP6602_H_ */
