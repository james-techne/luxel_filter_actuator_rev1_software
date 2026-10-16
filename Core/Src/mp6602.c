/**
 * @file mp6602.c
 * @brief MPS MP6602 stepper motor driver routines.
 *
 * Implementation of the interface in mp6602.h.
 *
 * @author james
 * @date Aug 19, 2026
 */

#include "../Inc/mp6602.h"
// Original value for 1st register is 0x1118	//Reset Vals
/**
 * @brief MP6602 register reset values.
 *
 * Eight 16-bit frames, one per register from CTRL to FAULT, stored as
 * {high byte, low byte} pairs. Used by mp6602_reset().
 */
uint8_t mp6602_reset_vals[16] = {0x11, 0x26,
	   	 0x30, 0x34,
	     0x52, 0x49,
	     0x78, 0x00,
	     0x93, 0x00,
	     0xB5, 0x10,
	     0xD5, 0x50,
	     0xF0, 0x02
};


void mp6602_init(void)
{
	HAL_GPIO_WritePin(GPIOB, MP6602_nRST_Pin, SET);

	uint8_t txData[2] = {0x11, 0x26};

	HAL_StatusTypeDef status;

	//Currently stepping at 1/16th microstepping
	txData[0] = 0x16;
	txData[1] = 0x11;
	HAL_GPIO_WritePin(GPIOA, SPI_CS_Pin, RESET);
	status = HAL_SPI_Transmit(&hspi1, txData, 2, 100);
	HAL_GPIO_WritePin(GPIOA, SPI_CS_Pin, SET);

	txData[1] = 0x30;
	txData[0] = 0x34;
	HAL_GPIO_WritePin(GPIOA, SPI_CS_Pin, RESET);
	status = HAL_SPI_Transmit(&hspi1, txData, 2, 100);
	HAL_GPIO_WritePin(GPIOA, SPI_CS_Pin, SET);

	txData[1] = 0x52;  // original: 0x52, max: 0x57
	txData[0] = 0x49;	// original: 0x49, max: 0xDF
	HAL_GPIO_WritePin(GPIOA, SPI_CS_Pin, RESET);
	status = HAL_SPI_Transmit(&hspi1, txData, 2, 100);
	HAL_GPIO_WritePin(GPIOA, SPI_CS_Pin, SET);

	txData[1] = 0x78;
	txData[0] = 0x00;
	HAL_GPIO_WritePin(GPIOA, SPI_CS_Pin, RESET);
	status = HAL_SPI_Transmit(&hspi1, txData, 2, 100);
	HAL_GPIO_WritePin(GPIOA, SPI_CS_Pin, SET);

	txData[1] = 0x93;
	txData[0] = 0x00;
	HAL_GPIO_WritePin(GPIOA, SPI_CS_Pin, RESET);
	status = HAL_SPI_Transmit(&hspi1, txData, 2, 100);
	HAL_GPIO_WritePin(GPIOA, SPI_CS_Pin, SET);

	txData[1] = 0xB5;
	txData[0] = 0x10;
	HAL_GPIO_WritePin(GPIOA, SPI_CS_Pin, RESET);
	status = HAL_SPI_Transmit(&hspi1, txData, 2, 100);
	HAL_GPIO_WritePin(GPIOA, SPI_CS_Pin, SET);

	txData[1] = 0xD5;
	txData[0] = 0x50;
	HAL_GPIO_WritePin(GPIOA, SPI_CS_Pin, RESET);
	status = HAL_SPI_Transmit(&hspi1, txData, 2, 100);
	HAL_GPIO_WritePin(GPIOA, SPI_CS_Pin, SET);

	txData[1] = 0xF0;
	txData[0] = 0x02;
	HAL_GPIO_WritePin(GPIOA, SPI_CS_Pin, RESET);
	status = HAL_SPI_Transmit(&hspi1, txData, 2, 100);
	HAL_GPIO_WritePin(GPIOA, SPI_CS_Pin, SET);
} //mp6602_init
/*
uint16_t mp6602_read(uint8_t reg)
{

} //mp6602_read

void mp6602_write(uint8_t reg, uint16_t val)
{

} //mp6602_write
*/
void mp6602_reset(void)
{
	uint8_t ii;
	HAL_StatusTypeDef status;


	for(ii = 0; ii<2; ii = ii+2)
	{
		HAL_GPIO_WritePin(GPIOA, SPI_CS_Pin, RESET);
		status = HAL_SPI_Transmit(&hspi1, &mp6602_reset_vals[ii], 2, 100);
		HAL_GPIO_WritePin(GPIOA, SPI_CS_Pin, SET);

	}
} //mp6602_reset

void mp6602_enable(void)
{
	//0x1126
	//STEP=DIR=1
	//EN = 0
	//
	uint8_t txData[2] = {0x11, 0x06};
	HAL_GPIO_WritePin(GPIOA, SPI_CS_Pin, RESET);
	HAL_SPI_Transmit(&hspi1, txData, 2, 100);
	HAL_GPIO_WritePin(GPIOA, SPI_CS_Pin, SET);

} //mp6602_enable
