

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define NUM_ALLOCS 10000
#define MAX_SIZE 100
#define MAX_ITERATIONS 10000


#define MIN_ALLOCATE 16
#define PAGE_SIZE  4096
#define SBRK_INCREMENT    (32*PAGE_SIZE)
#define SBRK_DECREMENT    (28*PAGE_SIZE)
#define MIN_FREE_BLOCK_SIZE (sizeof(size_t)+MIN_ALLOCATE)

typedef struct node { 
size_t block_size ;					
struct node* next 	; 
struct node* prev 	;
}node_t ;



// my free list 
static node_t *head = NULL;
// traverse free list function


int init_freelist(void)
{	
	
    if (NULL == head) {
	head = (node_t *) sbrk(SBRK_INCREMENT);
	if (head == (void *) -1) {
	    return -1;
	} else {
	    head->block_size = SBRK_INCREMENT - sizeof(size_t);
	    head->next = NULL;
	    head->prev = NULL;
	    return 0;
	}
    } else {
	return -2;
    }
}

void *split(node_t * node, size_t needed_size)
{

    if (node->prev == NULL) {
	//split head      
	node_t *new_node =(node_t*) (((unsigned char *) node +(needed_size + sizeof(size_t))));
	new_node->block_size =
	    node->block_size - (needed_size + sizeof(size_t));
	node->block_size = needed_size;	// set the size of allocated block
	
	new_node->next = head->next;

	if (head->next != NULL){
	head->next->prev = new_node ; // last bug isa 
	}
	new_node->prev = NULL  ;
	head = new_node;

	return (void *) node;

    } 
	else if (node->next == NULL) {
	node_t *new_node = (node_t *) ((unsigned char *) node + needed_size + sizeof(size_t));	//shift the pointer 
	// set the sizes of new_node and node
	new_node->block_size =
	    node->block_size - (needed_size + sizeof(size_t));
	node->block_size = needed_size;	// set the size of allocated block
	//make the next of new_node to NULL 
	
	
	new_node->next = NULL;

	// make the prev of node points to the new node 
	
	node->prev->next = new_node;
	new_node->prev = node->prev ; // edited 

	return (node_t *) node;	// return the pointer of allocated 

    } else {
	node_t *new_node = (node_t *) ((unsigned char *) node + needed_size + sizeof(size_t));	//shift the pointer 
	// set the sizes of new_node and node
	new_node->block_size =
	    node->block_size - (needed_size + sizeof(size_t));
	node->block_size = needed_size;	// set the size of allocated block

	new_node->prev = node->prev ;
	new_node->next = node->next ;	// make the next of new_node to the next of node
	 
	node->prev->next = new_node ;	// make the prev of node points to the new node  
	node->next->prev = new_node ;

	return (node_t *) node;	// return the pointer of allocated 
    }
}

void *traverse_freelist(node_t * node, size_t needed_size)
{	while (node != NULL){
    if (needed_size == node->block_size) {
	if (node->prev == NULL) {
		
	    head = node->next;

		if (head != NULL){
			
			head->prev = NULL;

		}

	    return (node_t *) node;

	} else if (node->next == NULL) {

	    node->prev->next = NULL;

	    return (node_t *) node;

	} else {

	    node->prev->next = node->next;
	    node->next->prev = node->prev;

	    return (node_t *) node;

	}
    } else if (node->block_size > needed_size) {
	if ((node->block_size - needed_size) >= MIN_FREE_BLOCK_SIZE) {

	    return split(node, needed_size);
	} else {
	    if (node->next == NULL)	//found that the next equals NULL 
	    {
		node_t *temp_node = (node_t *) sbrk(SBRK_INCREMENT);	//move the SBRK

		if ((void *) temp_node == (void *) -1) {
		    return NULL;
		} else {
		    // adjust the metadata of new node 
		    temp_node->block_size =
			SBRK_INCREMENT - sizeof(size_t);
		    if ((node_t *) ((unsigned char *) node +
				    (node->block_size + sizeof(size_t))) ==
			temp_node) {
			//merge them 
			node->block_size +=
			    temp_node->block_size + sizeof(size_t);
			

		    } else {

			node->next = temp_node;	// add the new_node to the free list
			temp_node->prev = node;
			temp_node->next = NULL;
			
			node = node->next ;
		    }
		}
	    } else {
			
			node = node->next ;
	    }

	}
    } else {
	if (node->next == NULL)	//found that the next equals NULL 
	{
	    node_t *temp_node = (node_t *) sbrk(SBRK_INCREMENT);	//move the SBRK 
	    if ((void *) temp_node == (void *) -1) {
		return NULL;
	    } else {
		// adjust the metadata of new node 
		temp_node->block_size = SBRK_INCREMENT - sizeof(size_t);
		if ((node_t *) ((unsigned char *) node +
				(node->block_size + sizeof(size_t))) ==
		    temp_node) {
		    //merge them 
		    node->block_size +=
			temp_node->block_size + sizeof(size_t);
		  // return traverse_freelist(node, needed_size);	//traverse the freelist 
		  continue ;
		} else {
		    node->next = temp_node;	// add the new_node to the free list
		    temp_node->prev = node;
		    temp_node->next = NULL;
		    //return traverse_freelist(node->next, needed_size);	//traverse the freelist
			node = node->next ; 
			continue ;
		}


	    }
	} else {
	   // return traverse_freelist(node->next, needed_size);
	   node = node->next ;
	   continue ;
	}
    }
}
}



void *malloc(size_t size)
{  	
    if (size < MIN_ALLOCATE) {
	size = MIN_ALLOCATE;
	}
    node_t *node_ptr = NULL;

    if (head == NULL) {
	if (!init_freelist()) {
	    node_ptr = traverse_freelist(head, size);

	} else {
	    // error can't init free_list
	    node_ptr = NULL;
	}
    } else {
	// there's a free list 
	node_ptr = traverse_freelist(head, size);
	}
    return (void *) ((unsigned char *) node_ptr + (sizeof(size_t)));
}

/*
*
* merge : merge two free nodes 
*
*
*/
void merge(node_t * left_node, node_t * right_node)
{	
    if ((node_t *) ((unsigned char *) left_node + (left_node->block_size) +
		    sizeof(size_t)) == (node_t *) right_node) {

	left_node->block_size +=
	    (right_node->block_size + (sizeof(size_t)));
	left_node->next = right_node->next;
	if (right_node->next != NULL) {
	    right_node->next->prev = left_node;
	}

    }
}

void check_on_tail_size(node_t * tail)
{
    void *shifted_tail =
	(void *) ((unsigned char *) tail + (tail->block_size) +
		  sizeof(size_t));
    if (tail->block_size >= (SBRK_DECREMENT - sizeof(size_t))) {
	void *current_pb = sbrk(0);
	if (shifted_tail == current_pb) {
	    if (tail->prev == NULL) {

			head = NULL ;

	    }else {
			
			tail->prev->next = NULL;
	    
		}

	    void *ret_ptr = sbrk(-(tail->block_size + (sizeof(size_t))));
	    if (ret_ptr == (void *) -1) {
		return;
		
	    }
	}
    }
}



void put_node_at_head(node_t * node)
{
    node->next = head;
	if (head != NULL){

		head->prev = node;
	}
    node->prev = NULL;
    head = node;
}

void put_node_between_two_nodes(node_t * node, node_t * next_node)
{
// bsm ellah el rahman el rahem
    node->prev = next_node->prev;				
    node->next = next_node;											
    next_node->prev->next = node;				
    next_node->prev = node;
}

void put_node_at_tail(node_t * node, node_t * tail)
{
    tail->next = node;
    node->next = NULL;
    node->prev = tail ;

}

void free(void *ptr)
{	
    char ret = 0;
    if (ptr == NULL) {
	return;
    }

    node_t *param_node =
	(node_t *) ((unsigned char *) ptr - (sizeof(size_t)));
    if (param_node == NULL) {
	return;
    }
    node_t *temp_node = head;

    if (temp_node == NULL) {

	put_node_at_head(param_node);

	if (param_node->next == NULL) {

	    check_on_tail_size(param_node);
	}
    } else {
	while (temp_node) {
	    if (temp_node == param_node) {
		break;
	    } 
		else if (temp_node > param_node) {
		if (temp_node == head) {
		    // add node to head 
		    put_node_at_head(param_node);
		    //merg with the next one 
		    merge(param_node, param_node->next);

		    if (param_node->next == NULL) { // check if merged 
			check_on_tail_size(param_node);
		    }
		    break;
		} else {
		    put_node_between_two_nodes(param_node, temp_node);	//add at the mid of nodes
		    merge(param_node, temp_node);	//merg with the previous and then merg with the next

		    node_t *dummy = param_node;
		    merge(param_node->prev, param_node);	//merge with the next
		    if (param_node->prev != NULL) {
			if (param_node->prev->next == NULL) {

			    check_on_tail_size(param_node->prev);
			}
		    }
		    break;
		}
	    } else {
		if (temp_node->next == NULL) {
		    // add at the end of free list 
		    put_node_at_tail(param_node, temp_node);
		    // merge the last node with the previous one
		    merge(temp_node, param_node);
		    // check if the size of last node 
		    if (temp_node->next == NULL) {
			//the temp_node and the param node merged
			check_on_tail_size(temp_node);
		    } else {
			//not merged 
				check_on_tail_size(temp_node->next);
		    }
		    break;
		}
	    }

	    temp_node = temp_node->next;
	}
    }
}

void *calloc(size_t nmemb, size_t size)
{	
    size_t total_size = (size * nmemb);

    void *ptr = malloc(total_size);	// allocate
	if (ptr == NULL) {
		return NULL ;
	}
	if (total_size == 0){
		total_size = MIN_ALLOCATE ;
	}
    memset(ptr, 0, total_size);
    return ptr;
	
}

void *realloc(void *old_ptr, size_t new_size)
{
    void *new_ptr = NULL ;

	size_t cpy_size ;
	size_t old_size = 0  ;

	new_ptr = malloc(new_size);	// allocate the new_size

	if (old_ptr){
    old_size =
	*(size_t *) ((unsigned char *) old_ptr - sizeof(size_t));
	cpy_size  =  (new_size > old_size) ? old_size : new_size  ; 
    memcpy(new_ptr, old_ptr, cpy_size);
	}

	free(old_ptr);

    return new_ptr;
}


void random_alloc_free_test() {
    srand((unsigned int)time(NULL));
    
    void* pointers[NUM_ALLOCS] = {NULL};
    
    for (int i = 0; i < MAX_ITERATIONS; ++i) {
        int index = rand() % NUM_ALLOCS;
        if (pointers[index] == NULL) {
            // Allocate memory
            size_t size = (size_t)(rand() % MAX_SIZE) + 1;
            pointers[index] = malloc(size);
            if (pointers[index] != NULL) {
                printf("Allocated memory of size %zu at address %p\n", size, pointers[index]);
            } else {
                fprintf(stderr, "Allocation failed for size %zu\n", size);
            }
        } else {
            // Free memory
            printf("Freeing memory at address %p\n", pointers[index]);
            free(pointers[index]);
            pointers[index] = NULL;
        }
    }
    
    // Free remaining allocated memory
    for (int i = 0; i < NUM_ALLOCS; ++i) {
        if (pointers[i] != NULL) {
            printf("Freeing remaining memory at address %p\n", pointers[i]);
            free(pointers[i]);
            pointers[i] = NULL;
        }
    }
}

int main() {
    printf("Starting random allocation and deallocation test...\n");
    random_alloc_free_test();
    printf("Test complete.\n");
    return 0;
}