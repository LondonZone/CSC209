/*
 ============================================================================
 Name        : smalloc.c
 Author      : David London
 Version     : 1
 Copyright   : copyright David London 2015
 Description : CSC209 Assignment 2  
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "smalloc.h"


/*
 Global variables 
 ============================================================================

 mem: stores the starting address of the memory region that is reserved by mem_init.

 freelist: A linked list of struct blocks that identify the portions of the memory   region that are free (not in use). Blocks in this list are stored in increasing address order.

allocated_list: A linked list of struct blocks that identify portions of memory that have been reserved by calls to smalloc. When a block is allocated it is placed at the front of this list, so the list is unordered.

============================================================================
*/

void *mem;
struct block *freelist;
struct block *allocated_list; //initialized empty

/* Initialize the memory space used by smalloc,
 * freelist, and allocated_list
 * Note:  mmap is a system call that has a wide variety of uses.  In our
 * case we are using it to allocate a large region of memory. 
 * - mmap returns a pointer to the allocated memory
 * Arguments:
 * - NULL: a suggestion for where to place the memory. We will let the 
 *         system decide where to place the memory.
 * - PROT_READ | PROT_WRITE: we will use the memory for both reading
 *         and writing.
 * - MAP_PRIVATE | MAP_ANON: the memory is just for this process, and 
 *         is not associated with a file.
 * - -1: because this memory is not associated with a file, the file 
 *         descriptor argument is set to -1
 * - 0: only used if the address space is associated with a file.
 */
void mem_init(int size) {
    mem = mmap(NULL, size,  PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
    if(mem == MAP_FAILED) {
         perror("mmap");
         exit(1);
    }

	//freelist.
	//allocated_list = &addr;
	struct block *node = malloc(sizeof(struct block));
	if(node == NULL) {
	 	perror("Malloc");
	    exit(1);
	 }
	
	node->addr = &mem;
	node->next = NULL;
	node->size = size;//access and assign size
	freelist = node; 
	//printf(node->addr);
	
	allocated_list = NULL; // check when mem_init is called is it only called once?
	
	
	  /* NOTE: this function is incomplete 
	 *To complete mem_init, you need to create a block node with the starting 
	address as given by mmap.
	
	 Therefore, after mem_init completes, the allocated_list will be empty (but initialized!), and freelist will have one block in it where the address contained in that block is the address returned by mmap.
	
	The starting address for the new mapping is specified in addr, which is NULL
	*/
        
}

/*
 ============================================================================
  - Calls free() to free all the dynamically allocated memory to:	 (allocated_list and  freelist) 

  - used by the program before exiting, to prevent a memory leak.
	
   - (The valgrind program must show all heap memory freed,
	 otherwise you have a memory leak. See below.)
 ============================================================================
*/
void mem_clean(){
	struct block *currentNode = freelist;
	
    while(currentNode != NULL){
		currentNode = currentNode->next;
		free(currentNode);
		freelist = currentNode;
    }

    currentNode = allocated_list;
    while(currentNode != NULL){
    	currentNode = currentNode->next;
    	free(allocated_list);
    	allocated_list = currentNode;
    }
	

}

/*
 ============================================================================
 - Reserves size bytes of space from the memory region created by mem_init. 
 - If the memory is reserved (allocated) successfully, Returns a 
   pointer to the reserved memory. 
 - Return NULL If the memory cannot be reserved (ie not enough to hold size bytes)
 ============================================================================
*/
void *smalloc(unsigned int nbytes) {
    if (nbytes > freelist->size) {
		return NULL;

	}else if (nbytes == freelist->size) {
		allocated_list = freelist;
		return allocated_list->addr;
		
	}else{
		//reassign size
		freelist->size = (freelist->size - nbytes);
		//initialize new block for allocated_list
		struct block *node = malloc(sizeof(struct block));
			if(node == NULL) {
				 perror("Malloc");
				 exit(1);
			}
		struct block *temp = allocated_list; //copy of LL 
		node->addr = freelist->addr; 
		node->next = temp; // next points to node added to front of LL
		node->size = nbytes;
		allocated_list = node;

		freelist->addr = (freelist->addr + nbytes);
		return node->addr;
	}
	
	
	

	// address of node added
	
}//end of smalloc

/*
 ============================================================================
  - Returns memory allocated by smalloc to the list of free blocks, 
	so that it might be reused later.
 ============================================================================
*/
//typedef currentNode curr;

int sfree(void *addr) {
	struct block *currentNode = allocated_list;
	struct block *prev;
	struct block *temp;
	//struct block *free = freelist;

    //check validity of memory address
   if(addr == NULL){
		return -1;
		
	}
		prev = NULL;
    	while(currentNode != NULL){
			if(currentNode->addr == addr){
				//Save the node we wish to delete in a temporary pointer
				temp = currentNode->next; 
				freelist->size += currentNode->size; 
				//munmap(currentNode->addr,currentNode->size);

				if(temp != NULL){
					currentNode->next = temp->next; 
					if(prev != NULL){
						prev->next = temp;
					}else{
						prev = temp;
					}
				}else{
					allocated_list = NULL;
				}
				
				
			/*Set the previous node's next pointer to point 
			to the currentNode's next (ie the block we wish to free) */
				//freelist->size += currentNode->size; 
				//munmap(currentNode->addr,currentNode->size);
				return 0;

			}else{
				prev = currentNode;
				currentNode = currentNode->next;//increment pointer
			}
			
		}	
		
	return -1; //unsuccessful sfree();
}

//end of sfree


