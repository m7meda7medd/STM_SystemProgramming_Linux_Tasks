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
      reminder_size = node->block_size - needed_size - sizeof(node_t); 
  if (reminder_size < MIN_FREE_BLOCK_SIZE ){
      // traverse for another node
      return NULL ; 
 }else{
      // make new free node 
      // split 
      node->block_size = needed_size ;
      node->block_state = ALLOCATED  ;
      node_t* temp_node = ((node_t*)((volatile unsigned char*) node + node->block_size + sizeof(node_t))) ;  
      temp_node->next = node->next ; 
      temp_node->prev = node ; 
      temp_node->block_size = reminder_size ;
      temp_node->block_state = FREE ;  
      node->next =  temp_node ;   
      return (void*) node ;   
 } 
}	

node_t* traverse_freelist (node_t* node , size_t needed_size) {
 if ( NULL == node->next /* in last node*/) {
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
      node_t* new_node = (node_t*) sbrk(SBRK_INCREMENT) ;
        if (new_node == (node_t*) -1)
        {
          return NULL ;
        }
        else{
            node->block_size+=SBRK_INCREMENT ;
            traverse_freelist(node,needed_size) ; 
        }
        }
        else{
          return ret_ptr ;  
        }
    }
    else{  // the  last node is free but have smaller size 
        node_t* new_node = (node_t*) sbrk(SBRK_INCREMENT) ;
        if (new_node == (node_t*) -1)
        {
          return NULL ; 
        }
        else{
            node->block_size+=SBRK_INCREMENT ;
            traverse_freelist(node,needed_size) ;  // traverse the last node again 
        }
    }
  } 
  else {  // the last node is allocated 
            node_t* new_node = (node_t*) sbrk(SBRK_INCREMENT) ;
            new_node->block_size=SBRK_INCREMENT - sizeof(node_t); 
            new_node->prev = node ; 
            new_node->block_state = FREE ; 
            new_node->next = NULL ; 
            node->next = new_node ;
            traverse_freelist(new_node,needed_size) ; // traverse from last node 
  }

 }else{
  if (node->block_state == FREE){
    if (node->block_size == needed_size){

        node->block_state = ALLOCATED ;

        return node ;  
      }
    else if (node->block_size > needed_size){
      // split 
      void* ret_ptr = NULL ;

      ret_ptr =  split(node,needed_size) ; 

      if (NULL == ret_ptr){
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
}

void* HMM_malloc(size_t size ){
if (0 == size ){
 return NULL ; 
}
if (NULL == head) {
  void* start =  sbrk(SBRK_INCREMENT) ;
  if (start == (void*) -1){
    return NULL ; 
  }
   if (init_freelist(start,size)) {
      while(1) ;
      }    
}
      node_t* node_ptr = traverse_freelist(head,size) ;
      if (node_ptr == NULL) {
        return NULL ; 
      }else{
      return (void*) ((unsigned char*) node_ptr + sizeof(node_t)) ; 
      }
} 
void merge(node_t* node ,  node_t* next_node) {

  node->block_size+= (next_node -> block_size + sizeof(node_t)) ;
  node->next = node->next->next ; 
  node->next->prev =  node ;

}
void HMM_free(void* ptr){
  node_t* node_ptr = NULL ; 
  if (ptr == NULL)
  {
    return ; 
  }
  node_ptr = (node_t*) ((unsigned char*) ptr - (sizeof(node_t)));
  if (node_ptr  == NULL){
    return ; 
  }else{
     node_t* temp_node = head ; 
    while(temp_node != NULL) {
      if (temp_node == node_ptr)
      {  if (temp_node->block_state == ALLOCATED){
            if (temp_node == head)
          {
            if (temp_node -> next == NULL)
            {
              temp_node->block_state = FREE ; 
              temp_node = temp_node->next ; 
            }
            else{
              merge(temp_node,temp_node->next) ; 
          }
        }
        else if ((temp_node->next == NULL)) { // last node and (not the first one) and the previous block is allocated

           if (temp_node -> block_size >= SBRK_INCREMENT){
              void* temp_ptr = sbrk(-(temp_node->block_size + sizeof(node_t))) ;
              if (temp_ptr == (void*)(-1))
              {
                while(1) ;
                return ; 
              }
           }
           else{
            temp_node->block_state = FREE ;   
           }
          }
        else{
              if (temp_node->next->block_state == FREE){
                
                merge(temp_node,temp_node->next) ; 
              }
              else {
                temp_node->block_state = FREE ;  
              }
        }     
      }
      else { 
        temp_node = temp_node->next ; 
      }
      }
      else{
         temp_node = temp_node->next ; 
      }
       
      }
  } 
}
/*
int main ()
{
  void* ptr = NULL ; 
void* init_pb = sbrk(0) ; 
void* ptr1 = HMM_malloc(24) ;
void* ptr2 = HMM_malloc(64) ; 
void* ptr3 = HMM_malloc(100) ; 
HMM_free(ptr2) ; 
void* ptr4 = HMM_malloc(4*4096) ;  
void* ptr5= HMM_malloc(7844) ;
HMM_free(ptr5) ;  
void* ptr6= HMM_malloc(500) ; 
HMM_free(ptr1) ; 
//ptr = ((unsigned char*)ptr2-(2*sizeof(size_t)))  ; 
//*((size_t*)(ptr)) = 0;  
void* ptr7 = HMM_malloc(30) ; 
HMM_free(ptr3) ; 
HMM_free(ptr4) ; 
HMM_free(ptr6) ; 
HMM_free(ptr7) ; 
return 0 ;



}
*/