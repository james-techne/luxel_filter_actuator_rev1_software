/*
 * cmd.c
 *
 *  Created on: Oct 6, 2026
 *      Author: james
 */

#include "../Inc/cmd.h"

void cmd_home(void)
{
	//Pull info out of UART_Rx buffer
	//Parse
	actuator_home();

} //cmd_home
void cmd_home_q(void)
{

} //cmd_home_q
void cmd_pos(void)
{
	//Pull info out of UART_Rx buffer
	//Parse
	uint8_t cmd, pos, term;
	CB_buffer_remove_item(UART_Rx, &cmd);
	CB_buffer_remove_item(UART_Rx, &pos);
	CB_buffer_remove_item(UART_Rx, &term);

	if((cmd != CMD_POS) || (term != CMD_TERM))
	{
		return;
	}

	pos = pos - 0x30;
	actuator_move_pos(pos);
} //cmd_pos
void cmd_pos_q(void)
{

} //cmd_pos_q
