/*
 * cmd.c
 *
 *  Created on: Oct 6, 2026
 *      Author: james
 */

#include "../Inc/cmd.h"

void cmd_home(void)
{
	uint8_t cmd_str[8];
	uint8_t ii;
	//Pull info out of UART_Rx buffer
	CB_buffer_remove_item(UART_Rx, &cmd_str[0]);
	CB_buffer_remove_item(UART_Rx, &cmd_str[1]);

	if(cmd_str[1] == '?')
	{
		if(gl_homed)
		{
			cmd_str[1] = '1';
		}
		else
		{
			cmd_str[1] = '0';
		}
		// ACK
		cmd_str[2] = ':';
		cmd_str[3] = 'A';
		cmd_str[4] = 'C';
		cmd_str[5] = 'K';
		cmd_str[6] = 0x0D;
		//cmd_str[6] = 0x0A;
		for(ii=0;ii<0xFF;ii++){}
		HAL_UART_Transmit(&huart2, cmd_str, 7, 500);


	}
	else
	{
		if((cmd_str[0] != CMD_HOME) || (cmd_str[1] != CMD_TERM))
		{
			cmd_str[1] = ':';
			cmd_str[2] = 'N';
			cmd_str[3] = 'A';
			cmd_str[4] = 'C';
			cmd_str[5] = 'K';
			cmd_str[6] = 0x0D;
			//cmd_str[7] = 0x0A;

			HAL_UART_Transmit(&huart2, cmd_str, 7, 500);
			return;
		}

		//Parse
		actuator_home();
		while(gl_inmotion){};			//Block while in motion

		// ACK
		cmd_str[1] = ':';
		cmd_str[2] = 'A';
		cmd_str[3] = 'C';
		cmd_str[4] = 'K';
		cmd_str[5] = 0x0D;
		//cmd_str[6] = 0x0A;
		for(ii=0;ii<0xFF;ii++){}
		HAL_UART_Transmit(&huart2, cmd_str, 6, 500);

	}




} //cmd_home

void cmd_pos(void)
{
	//Pull info out of UART_Rx buffer
	//Parse
	uint8_t cmd_str[8];
	uint8_t pos, ii;
	CB_buffer_remove_item(UART_Rx, &cmd_str[0]);
	CB_buffer_remove_item(UART_Rx, &cmd_str[1]);
	CB_buffer_remove_item(UART_Rx, &cmd_str[2]);
	if(!gl_inmotion)
	{
		//If this is a query
		if(cmd_str[1] == '?')
		{
			pos = gl_filter_position + 0x30;
			cmd_str[1] = pos;
		}
		else	//If this is a command
		{
			pos = cmd_str[1] - 0x30;
			if((cmd_str[0] != CMD_POS) || (cmd_str[2] != CMD_TERM) || (pos > 4) || (pos < 1))
			{
				cmd_str[2] = ':';
				cmd_str[3] = 'N';
				cmd_str[4] = 'A';
				cmd_str[5] = 'C';
				cmd_str[6] = 'K';
				cmd_str[7] = 0x0D;

				for(ii=0;ii<0xFF;ii++){}
				HAL_UART_Transmit(&huart2, cmd_str, 8, 500);
				return;
			}

			actuator_move_pos(pos);
			while(gl_inmotion){};			//Block while in motion
		}


		// ACK
		cmd_str[2] = ':';
		cmd_str[3] = 'A';
		cmd_str[4] = 'C';
		cmd_str[5] = 'K';
		cmd_str[6] = 0x0D;
		for(ii=0;ii<0xFF;ii++){}
		HAL_UART_Transmit(&huart2, cmd_str, 7, 500);
	}
	else
	{
		cmd_str[2] = ':';
		cmd_str[3] = 'N';
		cmd_str[4] = 'A';
		cmd_str[5] = 'C';
		cmd_str[6] = 'K';
		cmd_str[7] = 0x0D;

		for(ii=0;ii<0xFF;ii++){}
		HAL_UART_Transmit_IT(&huart2, cmd_str, 8);
		return;
	}

} //cmd_pos

