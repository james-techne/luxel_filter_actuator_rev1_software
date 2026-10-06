/**
 * @file circbuf.h
 * @brief Function prototypes for circbuf.c
 * Contains function protoypes for circular buffer routines
 * @author James Phillips
 * @date 6/28/2017
 */
#ifndef CIRCBUFF_H
#define CIRCBUFF_H

#include <stdint.h>
#include <stdlib.h>

typedef struct{
     uint8_t *buff;
     uint8_t head;
     uint8_t tail;
     uint16_t size;
     uint16_t count;
} CB_t;

 //typedef struct CB_t CB_t;

typedef enum{
     Success,
     BuffFull,
     BuffEmpty,
     NullError
} CB_status_e;

 /**@brief Adds data to circular buffer
  *Given a pointer to buffer and data to add, adds data to buffere and returns
  *Success or Failure as an enum
  * @param
  * Inputs: Pointer to circular buffer, data to be added to the circular buffer
  * @return - Enum specifying success/failure
  */
CB_status_e CB_buffer_add_item(CB_t *buf_struct, uint8_t data);

  /**@brief Removes data from circular buffer
   *Given a pointer to buffer and data to add, adds data to buffere and returns
   *Success or Failure as an enum
   * @param
   * Inputs: Pointer to circular buffer from which to remove data item,
   * Variable to store and return the removed item from buffer
   * @return - Enum specifying success/failure
   */
CB_status_e CB_buffer_remove_item(CB_t *buf_struct, uint8_t *data);

/**@brief Checks if buffer is full
 *Checks if the buffer passed is full and returns status or error codes
 * @param
 * Inputs: Circular buffer to check
 * @return - Status of circular buffer or error codes
 */
CB_status_e CB_is_full(CB_t *buf_struct);

/**@brief Checks if buffer is empty
 *Checks if the buffer passed is empty and returns status or error codes
 * @param
 * Inputs: Circular buffer to check
 * @return - Status of circular buffer or error codes
 */
CB_status_e CB_is_empty(CB_t *buf_struct);

/**@brief Peeks into buffer
 *Peaks at head of circular buffer
 * @param
 * Inputs: Pointer to the circular buffer to peak into, and the position relative
 * to the head of the buffer
 * @return - Status of circular buffer or error codes
 */
uint8_t CB_peek(CB_t *buf_struct, uint8_t Pos);

/**@brief Checks if buffer is full
 *Checks if the buffer passed is full and returns status or error codes
 * @param
 * Inputs: Pointer to the circluar buffer to check and the length to be reserved
 * in buffer
 * @return - Status of circular buffer or error codes
 */
CB_status_e CB_init(CB_t **buf_struct, uint16_t length);
/**@brief Checks if buffer is full
 *Checks if the buffer passed is full and returns status or error codes
 * @param
 * Inputs: Buffer to be destroyed
 * @return - Status of circular buffer or error codes
 */
CB_status_e CB_destroy(CB_t *buf_struct);

/**@brief Checks if buffer is full
 *Checks if the buffer passed is full and returns status or error codes
 * @param
 * Inputs: Buffer to be destroyed
 * @return - Status of circular buffer or error codes
 */
 CB_status_e CB_purge(CB_t *buf_struct);

#endif
