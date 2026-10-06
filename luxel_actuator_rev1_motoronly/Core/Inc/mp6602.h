/*
 * mp6602.h
 *
 *  Created on: Aug 19, 2026
 *      Author: james
 */

#ifndef INC_MP6602_H_
#define INC_MP6602_H_

#include <stdio.h>
#include <stdint.h>
#include "./main.h"

extern SPI_HandleTypeDef hspi1;

// Maybe include an array of values for each register to initialize
// the chip on board start up



// Register Read/Write
#define CTRL_READ	0x0000
#define CTRL_WRITE	0x1000
#define CTRL2_READ	0x2000
#define CTRL2_WRITE	0x3000
#define ISET_READ	0x4000
#define ISET_WRITE	0x5000
#define STALL_READ	0x6000
#define STALL_WRTE	0x7000
#define BEMF_READ	0x8000
#define BEMF_WRITE	0x9000
#define TSTP_READ	0xA000
#define TSTP_WRITE	0xB000
#define OCP_READ	0xC000
#define	OCP_WRITE	0xD000
#define FAULT_READ	0xE000
#define FAULT_WRITE	0xF000

// CTRL Masks
#define CTRL_EN		0x0001
#define CTRL_STEP	0x0002
#define CTRL_DIR	0x0004
// Microstepping (MS)
#define CTRL_MS_FULL	0x0038
#define CTRL_MS_HALF	0x0030
#define CTRL_MS_QUART	0x0008
#define CTRL_MS_8TH		0x0018
#define CTRL_MS_16TH	0x0020
#define CTRL_MS_32ND	0x0028
//Off time (OT)
#define CTRL_OT_20US	0x01C0
#define CTRL_OT_25US	0x0040
#define CTRL_OT_30US	0x0080
#define CTRL_OT_35US	0x00C0
#define CTRL_OT_40US	0x0100
#define CTRL_OT_45US	0x0140
#define CTRL_OT_50US	0x0180
#define CTRL_OT_55US	0x01C0
#define CTRL_AH_DIS		0x0E00
// Fill in the rest of the Automatic Hold (AH)


// Function Propotypes
uint16_t mp6602_read(uint8_t reg);
void mp6602_write(uint8_t reg, uint16_t val);
void mp6602_init(void);
void mp6602_enable(void);
void mp6602_reset(void);
void mp6602_faulthandle(void);
void mp6602_init(void);








#endif /* INC_MP6602_H_ */

