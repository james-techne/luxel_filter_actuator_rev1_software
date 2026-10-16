/**
 * @file circbuff.h
 * @brief Byte circular (ring) buffer.
 *
 * Types and function prototypes for the routines in circbuff.c. Used for
 * the USART2 receive and transmit buffers.
 *
 * @author James Phillips
 * @date 6/28/2017
 */
#ifndef CIRCBUFF_H
#define CIRCBUFF_H

#include <stdint.h>
#include <stdlib.h>

/**
 * @brief Circular buffer control structure.
 *
 * Allocated on the heap by CB_init() and released by CB_destroy().
 * @note @c head and @c tail are 8-bit indices, so @c size must not exceed 256.
 */
typedef struct{
     uint8_t *buff;	///< Heap-allocated storage array.
     uint8_t head;	///< Index of the next slot to write.
     uint8_t tail;	///< Index of the next slot to read.
     uint16_t size;	///< Capacity in bytes.
     uint16_t count;	///< Number of bytes currently stored.
} CB_t;

 //typedef struct CB_t CB_t;

/**
 * @brief Status codes returned by the circular buffer routines.
 */
typedef enum{
     Success,	///< Operation succeeded; buffer is neither full nor empty (status queries).
     BuffFull,	///< Buffer is full.
     BuffEmpty,	///< Buffer is empty.
     NullError	///< NULL pointer passed, or heap allocation failed.
} CB_status_e;

/**
 * @brief Add one byte at the head of a circular buffer.
 * @param buf_struct Pointer to the buffer.
 * @param data       Byte to store.
 * @retval Success   Byte added.
 * @retval BuffFull  Buffer is full; the byte was discarded.
 * @retval NullError @p buf_struct is NULL.
 */
CB_status_e CB_buffer_add_item(CB_t *buf_struct, uint8_t data);

/**
 * @brief Remove the oldest byte from a circular buffer.
 * @param[in]  buf_struct Pointer to the buffer.
 * @param[out] data       Receives the removed byte.
 * @retval Success   A byte was removed and written to @p data.
 * @retval BuffEmpty Buffer is empty; @p data is unchanged.
 * @retval NullError @p buf_struct or @p data is NULL.
 */
CB_status_e CB_buffer_remove_item(CB_t *buf_struct, uint8_t *data);

/**
 * @brief Check whether a circular buffer is full.
 * @param buf_struct Pointer to the buffer.
 * @retval BuffFull  Buffer is full.
 * @retval Success   Buffer has free space.
 * @retval NullError @p buf_struct is NULL.
 */
CB_status_e CB_is_full(CB_t *buf_struct);

/**
 * @brief Check whether a circular buffer is empty.
 * @param buf_struct Pointer to the buffer.
 * @retval BuffEmpty Buffer is empty.
 * @retval Success   Buffer holds at least one byte.
 * @retval NullError @p buf_struct is NULL.
 */
CB_status_e CB_is_empty(CB_t *buf_struct);

/**
 * @brief Read a byte without removing it.
 *
 * Returns the byte @p Pos places after the tail (the oldest byte). The
 * buffer contents and indices are left unchanged.
 * @param buf_struct Pointer to the buffer.
 * @param Pos        Offset from the tail; 0 returns the oldest byte.
 * @return The byte at the requested offset, or 0 if @p buf_struct is NULL.
 * @warning The offset does not wrap at the end of the storage array and is
 *          not checked against the number of stored bytes.
 */
uint8_t CB_peek(CB_t *buf_struct, uint8_t Pos);

/**
 * @brief Allocate and initialize a circular buffer on the heap.
 * @param[out] buf_struct Address of the caller's ::CB_t pointer; set to the new buffer.
 * @param      length     Capacity in bytes (at most 256, see ::CB_t).
 * @retval Success   Buffer created.
 * @retval NullError Allocation of the structure or its storage failed.
 */
CB_status_e CB_init(CB_t **buf_struct, uint16_t length);

/**
 * @brief Free a circular buffer and its storage.
 * @param buf_struct Buffer created by CB_init(); must not be used afterwards.
 * @retval Success   Buffer freed.
 * @retval NullError @p buf_struct is NULL.
 */
CB_status_e CB_destroy(CB_t *buf_struct);

/**
 * @brief Discard all data in a circular buffer.
 *
 * Moves the tail to the head and clears the count. Storage is not freed.
 * @param buf_struct Pointer to the buffer (not checked for NULL).
 * @retval Success Always.
 */
 CB_status_e CB_purge(CB_t *buf_struct);

#endif
