

#include "hmm.h"

static node_t *head = NULL;


static pthread_mutex_t hmm_mutex = PTHREAD_MUTEX_INITIALIZER;


/**
 * @brief Initialize the free list.
 *
 * @return 0 on success, -1 on failure.
 */
int
init_freelist (void)
{
  head = (node_t *) sbrk (SBRK_INCREMENT);
  if (head == (void *) -1)
    {
      return -1;
    }
  else
    {
      head->block_size = SBRK_INCREMENT - sizeof (size_t);
      head->next = NULL;
      head->prev = NULL;
      return 0;
    }
}


/**
 * @brief Add a node to the head of the free list.
 *
 * @param node Pointer to the node to be added.
 */

void
put_node_at_head (node_t * node)
{
  node->next = head;
  if (head != NULL)
    {

      head->prev = node;
    }
  node->prev = NULL;

  head = node;
}


/**
 * @brief Add a node between two nodes in the free list.
 *
 * @param node Pointer to the node to be added.
 * @param next_node Pointer to the next node in the list.
 */

void
put_node_between_two_nodes (node_t * node, node_t * next_node)
{
  /*  Put Node between two nodes */
  node->prev = next_node->prev;
  node->next = next_node;

  next_node->prev->next = node;
  next_node->prev = node;
}

/**
 * @brief Add a node at the tail of the free list.
 *
 * @param node Pointer to the node to be added.
 * @param tail Pointer to the tail of the list.
 */

void
put_node_at_tail (node_t * node, node_t * tail)
{
  tail->next = node;

  node->next = NULL;
  node->prev = tail;

}


/**
 * @brief Split a free node into two nodes.
 *
 * @param node Pointer to the node to be split.
 * @param needed_size Size needed for allocation.
 *
 * @return Pointer to the allocated block.
 */


void *
split (node_t * node, size_t needed_size)
{

  if (node->prev == NULL)
    {
      //split head      
      node_t *new_node =
	(node_t
	 *) (((unsigned char *) node + (needed_size + sizeof (size_t))));
      new_node->block_size =
	node->block_size - (needed_size + sizeof (size_t));
      node->block_size = needed_size;	// set the size of allocated block

      new_node->next = head->next;

      if (head->next != NULL)
	{
	  head->next->prev = new_node;	// last bug isa 
	}
      new_node->prev = NULL;
      head = new_node;

      return (void *) node;

    }
  else if (node->next == NULL)
    {
      node_t *new_node = (node_t *) ((unsigned char *) node + needed_size + sizeof (size_t));	//shift the pointer 
      // set the sizes of new_node and node
      new_node->block_size =
	node->block_size - (needed_size + sizeof (size_t));
      node->block_size = needed_size;	// set the size of allocated block
      //make the next of new_node to NULL 


      new_node->next = NULL;

      // make the prev of node points to the new node 

      node->prev->next = new_node;
      new_node->prev = node->prev;	// edited 

      return (node_t *) node;	// return the pointer of allocated 

    }
  else
    {
      node_t *new_node = (node_t *) ((unsigned char *) node + needed_size + sizeof (size_t));	//shift the pointer 
      // set the sizes of new_node and node
      new_node->block_size =
	node->block_size - (needed_size + sizeof (size_t));

      node->block_size = needed_size;	// set the size of allocated block

      new_node->prev = node->prev;
      new_node->next = node->next;	// make the next of new_node point to the next of node

      node->prev->next = new_node;	// make the prev of node points to the new node  
      node->next->prev = new_node;

      return (node_t *) node;	// return the pointer of allocated 
    }
}

/**
 * @brief Traverse the free list to find a suitable block for allocation.
 *
 * @param node Pointer to the starting node of the list.
 * @param needed_size Size needed for allocation.
 *
 * @return Pointer to the allocated node .
 */

void *
traverse_freelist (node_t * node, size_t needed_size)
{
  while (node != NULL)
    {
      if (needed_size == node->block_size)
	{
	  if (node->prev == NULL)
	    {

	      head = node->next;

	      if (head != NULL)
		{

		  head->prev = NULL;

		}

	      return (node_t *) node;

	    }
	  else if (node->next == NULL)
	    {

	      node->prev->next = NULL;

	      return (node_t *) node;

	    }
	  else
	    {

	      node->prev->next = node->next;
	      node->next->prev = node->prev;

	      return (node_t *) node;

	    }
	}
      else if (node->block_size > needed_size)
	{
	  if ((node->block_size - needed_size) >= MIN_FREE_BLOCK_SIZE)
	    {

	      return split (node, needed_size);

	    }
	  else
	    {
	      if (node->next == NULL)	//found that the next equals NULL 
		{
		  node_t *temp_node = (node_t *) sbrk (SBRK_INCREMENT);	//move the SBRK

		  if ((void *) temp_node == (void *) -1)
		    {
		      return NULL;
		    }
		  else
		    {
		      // adjust the metadata of new node 
		      temp_node->block_size =
			SBRK_INCREMENT - sizeof (size_t);
		      if ((node_t *) ((unsigned char *) node +
				      (node->block_size + sizeof (size_t))) ==
			  temp_node)
			{

			  //merge them 

			  node->block_size +=
			    temp_node->block_size + sizeof (size_t);

			  continue;
			}
		      else
			{

			  put_node_at_tail (temp_node, node);

			  node = node->next;	// see the next node 
			}
		    }
		}
	      else
		{

		  node = node->next;
		}

	    }
	}
      else
	{
	  if (node->next == NULL)	//found that the next equals NULL 
	    {
	      node_t *temp_node = (node_t *) sbrk (SBRK_INCREMENT);	//move the SBRK 
	      if ((void *) temp_node == (void *) -1)
		{
		  return NULL;
		}
	      else
		{
		  // adjust the metadata of new node 
		  temp_node->block_size = SBRK_INCREMENT - sizeof (size_t);
		  if ((node_t *) ((unsigned char *) node +
				  (node->block_size + sizeof (size_t))) ==
		      temp_node)
		    {
		      //merge them 
		      node->block_size +=
			temp_node->block_size + sizeof (size_t);

		    }
		  else
		    {

		      put_node_at_tail (temp_node, node);

		      node = node->next;
		    }


		}
	    }
	  else
	    {
	      node = node->next;
	    }
	}
    }
  return NULL;
}

/**
 * @brief Allocate memory.
 *
 * @param size Size of the memory to be allocated.
 *
 * @return Pointer to the allocated memory.
 */

void *
malloc (size_t size)
{
  pthread_mutex_lock (&(hmm_mutex));
  if (size < MIN_ALLOCATE)
    {
      size = MIN_ALLOCATE;
    }
  else if (size >= SIZE_MAX)
    {
      pthread_mutex_unlock (&(hmm_mutex));
      return NULL;
    }
  else
    {
      //to allign the size to 8 bytes // for 64 bit machine 
      size = ((size + 7) / 8) * 8;
    }

  node_t *node_ptr = NULL;

  if (head == NULL)
    {
      if (!init_freelist ())
	{
	  node_ptr = traverse_freelist (head, size);

	}
      else
	{
	  // error can't init free_list
	  pthread_mutex_unlock (&(hmm_mutex));
	  node_ptr = NULL;
	}
    }
  else
    {
      // there's a free list 
      node_ptr = traverse_freelist (head, size);
    }
  pthread_mutex_unlock (&(hmm_mutex));
  return (void *) ((unsigned char *) node_ptr + (sizeof (size_t)));

}

/**
 * @brief Merge two free nodes.
 *
 * @param left_node Pointer to the left node.
 * @param right_node Pointer to the right node.
 */

void
merge (node_t * left_node, node_t * right_node)
{
  if ((node_t *) ((unsigned char *) left_node + (left_node->block_size) +
		  sizeof (size_t)) == (node_t *) right_node)
    {

      left_node->block_size += (right_node->block_size + (sizeof (size_t)));

      left_node->next = right_node->next;
      if (right_node->next != NULL)
	{
	  right_node->next->prev = left_node;
	}

    }
}

/**
 * @brief Check if the size of the tail node is sufficient to release memory.
 *
 * @param tail Pointer to the tail node.
 */

void
check_on_tail_size (node_t * tail)
{
  void *shifted_tail =
    (void *) ((unsigned char *) tail + (tail->block_size) + sizeof (size_t));
  if (tail->block_size >= (SBRK_DECRMENT - sizeof (size_t)))
    {
      void *current_pb = sbrk (0);
      if (shifted_tail == current_pb)
	{
	  if (tail->prev == NULL)
	    {

	      head = NULL;

	    }
	  else
	    {

	      tail->prev->next = NULL;

	    }
	  signed long long decrement =
	    (signed long long) -(tail->block_size + (sizeof (size_t)));
	  void *ret_ptr = sbrk (decrement);
	  if (ret_ptr == (void *) -1)
	    {
	      return;

	    }
	}
    }
}


/**
 * @brief Free allocated memory.
 *
 * @param ptr Pointer to the memory to be freed.
 */

void
free (void *ptr)
{
  pthread_mutex_lock (&(hmm_mutex));
  if (ptr == NULL)
    {
      pthread_mutex_unlock (&(hmm_mutex));
      return;
    }

  node_t *param_node = (node_t *) ((unsigned char *) ptr - (sizeof (size_t)));
  if (param_node == NULL)
    {
      pthread_mutex_unlock (&(hmm_mutex));
      return;
    }
  node_t *temp_node = head;

  if (temp_node == NULL)
    {

      put_node_at_head (param_node);

      if (param_node->next == NULL)
	{

	  check_on_tail_size (param_node);
	}
    }
  else
    {
      while (temp_node)
	{
	  if (temp_node == param_node)
	    {
	      break;
	    }
	  else if (temp_node > param_node)
	    {
	      if (temp_node == head)
		{
		  // add node to head 
		  put_node_at_head (param_node);
		  //merg with the next one 
		  merge (param_node, param_node->next);

		  if (param_node->next == NULL)
		    {		// check if merged 
		      check_on_tail_size (param_node);
		    }
		  break;
		}
	      else
		{
		  put_node_between_two_nodes (param_node, temp_node);	//add at the mid of nodes

		  merge (param_node, temp_node);	//merg with the previous and then merg with the next

		  merge (param_node->prev, param_node);	//merge with the next

		  if (param_node->prev->next == NULL)
		    {

		      check_on_tail_size (param_node->prev);
		    }
		  break;
		}
	    }
	  else
	    {
	      if (temp_node->next == NULL)
		{
		  // add at the end of free list 
		  put_node_at_tail (param_node, temp_node);
		  // merge the last node with the previous one
		  merge (temp_node, param_node);
		  // check if the size of last node 
		  if (temp_node->next == NULL)
		    {
		      //the temp_node and the param node merged
		      check_on_tail_size (temp_node);

		    }
		  else
		    {
		      //not merged 
		      check_on_tail_size (temp_node->next);
		    }

		}
	    }

	  temp_node = temp_node->next;
	}
    }
  pthread_mutex_unlock (&(hmm_mutex));
}

/**
 * @brief Allocate memory initialized to zero.
 *
 * @param nmemb Number of members.
 * @param size Size of each member.
 *
 * @return Pointer to the allocated memory.
 */

void *
calloc (size_t nmemb, size_t size)
{
  size_t total_size = (size * nmemb);

  void *ptr = malloc (total_size);	// allocate
  if (ptr == NULL)
    {
      return NULL;
    }
  else if (total_size == 0)
    {
      total_size = MIN_ALLOCATE;
    }

  memset (ptr, 0, total_size);
  return ptr;

}

void
realloc_extend_block (void *ptr, size_t needed_size,
		      unsigned char *extend_split_flag)
{
  node_t *node_ptr = (node_t *) ((unsigned char *) ptr - sizeof (size_t));
  node_t *next_node_ptr =
    (node_t *) ((unsigned char *) ptr + node_ptr->block_size);
  size_t extended_size = needed_size - node_ptr->block_size;
  if ((head == NULL) || (next_node_ptr < head))
    {
      return;

    }
  else if (next_node_ptr == head)
    {
      //extend it with head

      if ((head->block_size > extended_size)
	  && ((head->block_size - extended_size) >= MIN_FREE_BLOCK_SIZE))
	{
	  node_t *shifted_node =
	    (node_t *) ((unsigned char *) head + (extended_size));

	  node_ptr->block_size += extended_size;

	  shifted_node->prev = NULL;
	  if (head->next != NULL)
	    {
	      head->next->prev = shifted_node;
	    }
	  shifted_node->next = head->next;
	  shifted_node->block_size =
	    head->block_size - extended_size - sizeof (size_t);
	  head = shifted_node;

	  *extend_split_flag = 1;
	}
    }
  else
    {
      node_t *temp_node = head->next;
      while (temp_node)
	{
	  if (next_node_ptr == temp_node)
	    {
	      // extend it with the next node
	      if ((temp_node->block_size > (extended_size))
		  && ((temp_node->block_size - extended_size) >=
		      MIN_FREE_BLOCK_SIZE))
		{
		  node_t *shifted_node =
		    (node_t *) ((unsigned char *) temp_node + extended_size);
		  node_ptr->block_size += extended_size;

		  temp_node->prev->next = shifted_node;
		  shifted_node->prev = temp_node->prev;
		  if (temp_node->next != NULL)
		    {
		      temp_node->next->prev = shifted_node;
		    }
		  shifted_node->next = temp_node->next;
		  shifted_node->block_size =
		    temp_node->block_size - extended_size - sizeof (size_t);


		  *extend_split_flag = 1;
		  break;
		}
	      else if (temp_node > next_node_ptr)
		{

		  break;
		}
	    }
	  temp_node = temp_node->next;
	}
    }
}

void
realloc_split_block (void *ptr, size_t needed_size,
		     unsigned char *extend_split_flag)
{
	
  node_t *node_ptr = (node_t *) ((unsigned char *) ptr - sizeof (size_t));
  size_t splitted_size = node_ptr->block_size - needed_size;

  if ((splitted_size >= MIN_FREE_BLOCK_SIZE)
      && (needed_size >= MIN_FREE_BLOCK_SIZE))
    {
      node_t *shifted_node = (node_t *) ((unsigned char *) ptr + needed_size);
      shifted_node->block_size = splitted_size - sizeof (size_t);
      node_ptr->block_size = needed_size;
      if (head == NULL)
	{
	  shifted_node->next = NULL;
	  shifted_node->prev = NULL;
	  head = shifted_node;
	}
      else
	{
	  node_t *temp_node = head;
	  while (temp_node)
	    {
	      if (temp_node > shifted_node)
		{

		  if (temp_node == head)
		    {
		      put_node_at_head (shifted_node);
		      merge (shifted_node, temp_node);
		      if (shifted_node->next == NULL)
			{
			  check_on_tail_size (shifted_node);
			}
		      *extend_split_flag = 1;
		      break;
		    }
		  else
		    {

		      put_node_between_two_nodes (shifted_node, temp_node);
		      merge (shifted_node, temp_node);
		      merge (shifted_node->prev, shifted_node);
		      if (shifted_node->prev != NULL)
			{
			  if (shifted_node->prev->next == NULL)
			    {
			      check_on_tail_size (shifted_node);
			    }
			}
		      *extend_split_flag = 1;
		      break;
		    }
		}
	      else
		{
		  if (temp_node->next == NULL)
		    {
		      // put it at the tail 
		      put_node_at_tail (shifted_node, temp_node);
		      merge (temp_node, shifted_node);
		      if (temp_node->next == NULL)
			{
			  check_on_tail_size (temp_node);
			}
		      else
			{
			  check_on_tail_size (shifted_node);
			}
		      *extend_split_flag = 1;
		      break;
		    }
		}
	      temp_node = temp_node->next;
	    }

	}
    }
}


/**
 * @brief Reallocate memory.
 *
 * @param old_ptr Pointer to the previously allocated memory.
 * @param new_size New size of the memory block.
 *
 * @return Pointer to the reallocated memory.
 */

void *
realloc (void *old_ptr, size_t new_size)
{
  void *new_ptr = NULL;
  unsigned char extend_or_split = 0;
  size_t cpy_size;
  size_t old_size = 0;
  new_size = (((new_size+7)/8)*8) ; 
  if ((old_ptr == NULL))
    {				// realloc(ptr,0)  == free(ptr)
      new_ptr = malloc (new_size);	// realloc(NULL,size) == malloc (size)

    }
  else if (new_size == 0)
    {
      free (old_ptr);
    }
  else
    {
      old_size = *(size_t *) ((unsigned char *) old_ptr - sizeof (size_t));

      if (new_size > old_size)
	{ 
	  pthread_mutex_lock (&hmm_mutex);
	  realloc_extend_block (old_ptr, new_size, &extend_or_split);
	  pthread_mutex_unlock (&hmm_mutex);
	  
	  cpy_size = old_size;

	}
      else if (old_size > new_size)
	{ 
	  pthread_mutex_lock (&hmm_mutex);
	  realloc_split_block (old_ptr, new_size, &extend_or_split);
	  pthread_mutex_unlock (&hmm_mutex);
	  cpy_size = new_size;

	}
      else
	{
	  new_ptr = old_ptr;
	  return new_ptr;
	}

      if (extend_or_split)
	{

	  new_ptr = old_ptr;

	}
      else
	{
	  new_ptr = malloc (new_size);	// allocate the new_size
	  if (new_ptr != NULL)
	    {
	      memcpy (new_ptr, old_ptr, cpy_size);
	      free (old_ptr);
	    }
	}
    }
  return new_ptr;
}
