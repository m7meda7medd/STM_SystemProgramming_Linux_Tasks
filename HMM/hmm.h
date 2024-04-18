#ifndef HMM_H
	#define HMM_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define MIN_ALLOCATE 16
#define PAGE_SIZE  4096
#define SBRK_INCREMENT    50000
#define SBRK_DECREMENT    10000
#define MIN_FREE_BLOCK_SIZE (sizeof(node_t)+MIN_ALLOCATE)

typedef struct node { 
size_t block_size ;
struct node* next 	; 
struct node* prev 	;
}node_t ;


void* HMM_malloc(size_t size );
void HMM_free(void* ptr) ;
void* HMM_realloc(void* old_ptr, size_t new_size) ;
void* HMM_calloc(size_t nmemb, size_t size) ;
























#endif 

