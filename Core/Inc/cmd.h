/*
 * cmd.h
 *
 *  Created on: Oct 6, 2026
 *      Author: james
 */

#ifndef INC_CMD_H_
#define INC_CMD_H_
#include <stdio.h>
#include <stdint.h>
#include "actuator.h"
#include "circbuff.h"
extern CB_t *UART_Rx;
extern CB_t *UART_Tx;
extern UART_HandleTypeDef huart2;


#define CMD_TERM		0x0D		//ENTER, carriage return
#define CMD_HOME		0x48		//H
#define CMD_HOME_LEN	2
#define CMD_HOME_Q		0x48		//H
#define CMD_HOME_Q_LEN	3
#define CMD_POS			0x50		//P
#define CMD_POS_LEN		3
#define CMD_POS_Q		0x50		//P
#define CMD_POS_Q_LEN	3


void cmd_home(void);

void cmd_pos(void);



#endif /* INC_CMD_H_ */
