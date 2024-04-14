#ifndef HMM_H
	#define HMM_H

#define MIN_ALLOCATE 64
#define PAGE_SIZE  4096
#define SBRK_INCREMENT    (unsigned long)(3*PAGE_SIZE)
#define MIN_FREE_BLOCK_SIZE (sizeof(node_t)+MIN_ALLOCATE)
// block states  
#define ALLOCATED 1 
#define FREE  0 

typedef struct node {
struct node* next ; 
struct node* prev ; 
size_t block_size  ;
size_t block_state ;
}node_t ;






















#endif 

