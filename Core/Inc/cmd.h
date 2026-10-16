/**
 * @file cmd.h
 * @brief Serial (USART2) command interpreter for the filter actuator.
 *
 * Commands arrive on USART2 (115200 baud, 8N1) and are collected in
 * ::UART_Rx by HAL_UART_RxCpltCallback(), which also echoes each received
 * character. A carriage return (::CMD_TERM) ends a command, and the main
 * loop dispatches on its first character.
 *
 * | Command  | Action                         | Reply                        |
 * |----------|--------------------------------|------------------------------|
 * | `H<CR>`  | Home the actuator              | `H:ACK<CR>`                  |
 * | `H?<CR>` | Query homed state              | `H1:ACK<CR>` or `H0:ACK<CR>` |
 * | `Pn<CR>` | Move to position n (1 to 4)    | `Pn:ACK<CR>`                 |
 * | `P?<CR>` | Query current position         | `Pn:ACK<CR>`                 |
 * | `C<CR>`  | Clear the UART buffers         | `C:ACK<CR>`                  |
 * | `R<CR>`  | Reset the microcontroller      | `R:ACK<CR>`                  |
 *
 * A malformed command is answered with the received characters followed by
 * `:NACK<CR>`. `H` and `P` commands, including the `?` queries, are refused
 * (NACK) while the actuator is moving; `P` commands are also refused before
 * homing or if the move would pass a software travel limit.
 *
 * @author james
 * @date Oct 6, 2026
 */

#ifndef INC_CMD_H_
#define INC_CMD_H_
#include <stdio.h>
#include <stdint.h>
#include "actuator.h"
#include "circbuff.h"
extern CB_t *UART_Rx;
extern CB_t *UART_Tx;
extern UART_HandleTypeDef huart2;	///< USART2 handle (defined in main.c) used for replies.


/**
 * @name Command characters and lengths
 * @{
 */
#define CMD_TERM		0x0D	///< Command terminator: carriage return (Enter).
#define CMD_HOME		0x48	///< 'H': home command.
#define CMD_HOME_LEN	2	///< Length of `H<CR>`.
#define CMD_HOME_Q		0x48	///< 'H': homed-state query (`H?`).
#define CMD_HOME_Q_LEN	3	///< Length of `H?<CR>`.
#define CMD_POS			0x50	///< 'P': move-to-position command.
#define CMD_POS_LEN		3	///< Length of `Pn<CR>`.
#define CMD_POS_Q		0x50	///< 'P': position query (`P?`).
#define CMD_POS_Q_LEN	3	///< Length of `P?<CR>`.
#define CMD_BUF_CLR		0x43	///< 'C': clear UART buffers.
#define CMD_RESET		0x52	///< 'R': reset the microcontroller.
#define CMD_WRITE		0x57	///< 'W': write (save) settings (reserved, not implemented).
#define CMD_GET			0x47	///< 'G': get settings (reserved, not implemented).
#define CMD_RESTORE		0x5A	///< 'Z': restore default position settings (reserved, not implemented).
/** @} */

/**
 * @brief Handle the `H` (home) command and the `H?` query.
 *
 * Removes the command bytes from ::UART_Rx. For `H?`, replies with the homed
 * state. For `H<CR>`, calls actuator_home(), blocks until motion stops, then
 * replies with an ACK. Anything else is answered with a NACK, and both the
 * command and the query are refused with `H:NACK<CR>` while the actuator is
 * moving.
 */
void cmd_home(void);

/**
 * @brief Handle the `Pn` (move) command and the `P?` query.
 *
 * If the actuator is homed and idle but fewer than three bytes are waiting
 * in ::UART_Rx, the command is treated as truncated: it is NACKed with
 * `P*:NACK<CR>` and the receive buffer is purged. Otherwise three bytes are removed from
 * ::UART_Rx. For `P?`, replies with the current
 * filter position. For `Pn<CR>` with n from 1 to 4, calls
 * actuator_move_pos(), blocks until motion stops, then replies with an ACK.
 * A NACK is sent if the command is malformed, n is not a digit from 1 to 4,
 * the actuator is moving, it has not been homed, or the move would pass a
 * software travel limit (see actuator_move_pos()).
 */
void cmd_pos(void);

/**
 * @brief Handle the `C` command: purge the UART receive and transmit buffers.
 *
 * Replies with an ACK. A malformed command is answered with a NACK, and the
 * receive buffer is purged.
 */
void cmd_buffer_clear(void);

/**
 * @brief Handle the `R` command: acknowledge, then reset the microcontroller.
 *
 * Performs a software reset with HAL_NVIC_SystemReset(), so it does not
 * return for a valid command. A malformed command is answered with a NACK,
 * and the receive buffer is purged.
 */
void cmd_reset(void);

/**
 * @brief Store settings (command `W`).
 * @note Declared but not yet implemented.
 */
void cmd_store(void);

/**
 * @brief Report stored settings (command `G`).
 * @note Declared but not yet implemented.
 */
void cmd_get(void);


#endif /* INC_CMD_H_ */
