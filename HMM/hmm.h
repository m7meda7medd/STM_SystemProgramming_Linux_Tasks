#ifndef HMM_H
        #define HMM_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define MIN_ALLOCATE 16
#define PAGE_SIZE  4096
#define SBRK_INCREMENT    (32*PAGE_SIZE)
#define SBRK_DECRMENT    (28*PAGE_SIZE)
#define MIN_FREE_BLOCK_SIZE (sizeof(size_t)+MIN_ALLOCATE)
#define SIZE_MAX 18446744073709551615UL
typedef struct node
{
  size_t block_size;
  struct node *next;
  struct node *prev;
} node_t;


void *malloc (size_t size);
void free (void *ptr);
void *realloc (void *old_ptr, size_t new_size);
void *calloc (size_t nmemb, size_t size);
























#endif
