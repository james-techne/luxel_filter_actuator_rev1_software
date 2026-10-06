/**
 * @file circbuf.c
 * @brief Circular buffer routines
 * Contains routines to manipulate and status circular buffer
 * @author James Phillips
 * @date 6/28/2017
 */

 #include "../Inc/circbuff.h"
 #include <stdlib.h>
 #include <stdint.h>


 CB_status_e CB_buffer_add_item(CB_t *buf_struct, uint8_t data){
   CB_status_e status = Success;
   //Check for null pointer
   if (buf_struct == 0){
     return status = NullError;
   }
   //Check if the buffer is full
   if ((buf_struct->count) == (buf_struct->size)){
     status = BuffFull;
     return status;
   }

   //Check if buffer head needs to wrap
   if(buf_struct->head == buf_struct->size)
   {
     buf_struct->head = 0;
   }

   //Add item to buffer
   *(buf_struct->buff + buf_struct->head) = data;
   buf_struct->head++;
   buf_struct->count++;

   return status;
 }//CB_buffer_add_item

 CB_status_e CB_buffer_remove_item(CB_t *buf_struct, uint8_t *data){
   CB_status_e status = Success;
   //Check for null pointers
   if ((buf_struct == 0) || (data == 0)){
     return status = NullError;
   }
   //Check if buffer is empty
   if ((buf_struct->count) == 0){
     status = BuffEmpty;
     return status;
   }

   //Check if buffer tail needs to wrap
   if(buf_struct->tail == buf_struct->size)
   {
     buf_struct->tail = 0;
   }

   //Remove item from buffer
   *data = *(buf_struct->buff + buf_struct->tail);
   buf_struct->tail++;
   buf_struct->count--;

   return status;
 }//CB_buffer_remove_item

 CB_status_e CB_is_full(CB_t *buf_struct){
   CB_status_e status = Success;
   //Check for null pointer
   if (buf_struct == 0){
     return status = NullError;
   }
   if ((buf_struct->count) == (buf_struct->size)){
     status = BuffFull;
   }

  return status;
}//CB_is_full

 CB_status_e CB_is_empty(CB_t *buf_struct){
   CB_status_e status = Success;
   //Check for null pointer
   if (buf_struct == 0){
     return status = NullError;
   }
   if ((buf_struct->count) == 0){
     status = BuffEmpty;
   }

   return status;
 }//CB_is_empty

 uint8_t CB_peek(CB_t *buf_struct, uint8_t Pos){
   //CB_status_e status = Success;   //Check for null pointer
   if (buf_struct == 0){
     return 0;
   }
   buf_struct->tail += Pos;

   uint8_t peekdat = *(buf_struct->buff + buf_struct->tail);
   buf_struct->tail -= Pos;


   return peekdat;
 }//CB_peek

 CB_status_e CB_init(CB_t **buf_struct, uint16_t length){
   CB_status_e status = Success;

   //Create the buffer structure
   *buf_struct = (CB_t *)malloc(sizeof(CB_t));

   //Structure point should no longer be NULL, should point to heap
   if(*buf_struct == NULL){
     return NullError;
   }

   //Initialize the structure
   (*buf_struct)->buff = malloc(sizeof(uint8_t)*length);
   (*buf_struct)->head = 0;
   (*buf_struct)->tail = 0;
   (*buf_struct)->size = length;
   (*buf_struct)->count = 0;

   //Check to see if buffer space was allocated on heap
   if ((*buf_struct)->buff == NULL){
     return NullError;
   }

   return status;
 }//CB_init

 CB_status_e CB_destroy(CB_t *buf_struct){
   CB_status_e status =  Success;
   if (buf_struct == NULL){
     return NullError;
   }
   //Deallocate buffer space
   free((void*)buf_struct->buff);
   //Deallocate uffer structure
   free((void*)(buf_struct));

   return status;
 }//CB_destroy

 CB_status_e CB_purge(CB_t *buf_struct)
 {
   CB_status_e status = Success;

   //To "empty" buffer, just make the tail point to the head
   buf_struct->tail = buf_struct->head;

   //Set count equal to 0
   buf_struct->count = 0;

   return status;

 }//CB_purge
