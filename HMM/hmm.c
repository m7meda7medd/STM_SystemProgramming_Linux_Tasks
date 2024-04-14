#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "hmm.h"
// my free list 
node_t * head  = NULL ;
// traverse free list function
unsigned char init_freelist (void* start_pb,size_t needed_size) {
if (NULL == head) {
  head = start_pb ;
  head->block_size = SBRK_INCREMENT - sizeof(node_t);
  head->next = NULL ; 
  head->prev = NULL ; 
  head->block_state = FREE ; 
  return 0  ;
}
return 1 ; 
}

void* split(node_t* node,size_t needed_size){
      // i have node that have block size larger than needed_size 
      size_t reminder_size ; 
      reminder_size = node->block_size - needed_size ; 
  if (reminder_size < MIN_FREE_BLOCK_SIZE ){
      // traverse for another node
      return NULL ; 
 }else{
      // make new free node 
      node->next =  (node_t*)((unsigned char*) node + node->block_size) ;
      node->next->block_size = reminder_size - sizeof(node_t) ;
      node->next->prev = node ; 
      node->next->next = NULL ; 
      node->next->block_state = FREE ;   
      // point to next free block     
      node->block_state = ALLOCATED ;
      return (void*) node ;   
 } 
}	
node_t* traverse_freelist (node_t* node , size_t needed_size) {
 if ( NULL == node ) {
  node_t* new_node = (node_t*) sbrk(SBRK_INCREMENT) ;
      if (node == (node_t*) -1)
      {
        perror("Error with SBRK") ; 
        exit(2) ;
      }
      else{
        node_t* last_node = head ; 
        while (last_node->next != NULL){
          last_node = last_node->next ; 
        }
        if (last_node->block_state == FREE){
          last_node->block_size+=SBRK_INCREMENT ;
          return last_node ; 
        }else {
          new_node->block_size=SBRK_INCREMENT - sizeof(node_t); 
          new_node->prev = last_node ; 
          new_node->block_state = FREE ; 
          new_node->next = NULL ; 
          last_node->next = new_node ;
        }
        
      }

 }
  if (node->block_state == FREE){
    if (node->block_size == needed_size){
        node->block_state = ALLOCATED ;
        return node ;  
      }
    else if (node->block_size > needed_size){
      // split 
      void* ret_ptr = NULL ; 
      ret_ptr =  split(node,needed_size) ; 
      if (NULL == ret_ptr)
      {
        traverse_freelist(node->next, needed_size) ;
      }
      else{
        return ret_ptr ;  
      }
    }
    else{ 
      traverse_freelist(node->next , needed_size) ; 
    }
  } 
  else { 
    traverse_freelist(node->next , needed_size) ; 
  }
}

void* HMM_malloc(size_t size ){
  size+=sizeof(size_t) ;
if (NULL == head) {
  void* start =  sbrk(SBRK_INCREMENT) ;
  if (start == (void*) -1 ){
    perror("Error with SBRK\n") ;
    exit(1)  ;
  } 
  else{
    if (!init_freelist(start,size)) {
      perror("Error to init free_list\n") ; 
      exit(1) ;
      }   
    else {
      node_t* ret_ptr = traverse_freelist(head,size) ;

      } 
}
}
return NULL ; 
}

int main ()
{
  /*
void* ptr1 = NULL ;
void* ptr2 = NULL ;  
ptr1 = HMM_malloc(64) ; 
if (ptr1 == NULL){
while(1) ; 
}
*/
return 0 ;



}
