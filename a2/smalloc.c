/*
 ============================================================================
 Name        : smalloc.c
 Author      : David London-Schacht
 Version     : 1
 Copyright   : copyright David London-Schacht 2015
 Description : CSC209 Assignment 2
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <assert.h>
#include "smalloc.h"


/*
 Global variables
 ============================================================================
 
 mem: stores the starting address of the memory region that is reserved by mem_init.
 
 freelist: A linked list of struct blocks that identify the portions of the memory  
 region that are free (not in use). Blocks in this list are stored in increasing address order.
 
 allocated_list: A linked list of struct blocks that identify portions of memory that 
have been reserved by calls to smalloc. When a block is allocated it is placed at the front of this list, so the list is unordered.
 
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
    
    //initialize linkedlist
    struct block *node = malloc(sizeof(struct block));
    if(node == NULL) {
        perror("Malloc Failed");
        exit(1);
    }
    
    node->addr = mem;
    node->next = NULL;
    node->size = size;//access and assign size
    freelist = node;
    
    //initialize  allocated_list to NULL after mem_init completes
    allocated_list = NULL; 
    
    
}

/*
 ============================================================================
 - Calls free() to free all the dynamically allocated memory to:  
   (allocated_list and  freelist)
 
 - used by the program before exiting, to prevent a memory leak.
	
 ============================================================================
 */
void mem_clean(){
 
   struct block *temp , *next;
   struct block *temp2 , *next2;
    
   //Traverse freelist and free memory allocated to each node
    while(freelist != NULL){
        temp  =  freelist;
        next  = freelist->next;  
        freelist  = freelist->next;
	free(temp);
        temp = next;
    }
    
    //Traverse allocated_list  and free memory allocated to each node
    while(allocated_list != NULL){
	temp2 = allocated_list;
        next2  = allocated_list->next;
        allocated_list  = allocated_list->next;
	free(temp2);
        temp2 = next2;
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
    struct block *current = freelist;
    struct block *prev;

    //Find block that has enough room to allocate memory
    prev = NULL;
    while(current != NULL && nbytes > current->size){
         prev = current;
         current = current->next;
    }
  
    // Case 1:  If no block in freelist have space to allocate nbytes
    if (current == NULL) {
        return NULL;
    //Case 2:  block of exactly the required size
    }else if (nbytes == current->size) {
        if(prev->next != NULL){ 
            prev->next = current->next;
            
        }else{
            freelist = current->next;
        }
        //insert new block into head of allocatedlist
        //current->next is a pointer to the block added
        current->next = allocated_list; 
        allocated_list = current;  
        return allocated_list->addr;
        
    }else{
        //Case 3  nbytes < current->size
        struct block *newNode = malloc(sizeof(struct block));
            if(newNode == NULL) {
                 perror("Malloc Failed");
                 exit(1);
            }
        //copy of LinkedList
        struct block *temp = allocated_list; 
        newNode->addr = freelist->addr;
        newNode->next = temp; // node added to the head of LL
        newNode->size = nbytes;
        allocated_list = newNode;

        //reassign size and address
        freelist->size = (freelist->size - nbytes);
        freelist->addr = (freelist->addr + nbytes);
        return newNode->addr;
    }
    
    
    
    
}

/*
 ============================================================================
 - Returns memory allocated by smalloc to the list of free blocks,
	so that it might be reused later.
 ============================================================================
 */

int sfree(void *addr) {
    struct block *currentNode = allocated_list;
    struct block *prev, * temp;
    //struct block *free = freelist;


    //check validity of memory address
     prev = NULL;
    while(currentNode != NULL){
        if(currentNode->addr == addr){
            //Save the next pointer to the node we wish to delete in a temporary pointer
            temp = currentNode->next;

            //insert node to be removed to freelist
	    (void) insertOrdered(currentNode,freelist);
	    
            //bypass block we want to free.
            if(temp != NULL){ // single node
                currentNode->next = temp->next;
                if(prev != NULL){       /* addr belongs to the first node */
                    prev->next = temp;
                }else{
		  prev = currentNode->next;  /* addr belongs to some other node*/
                }
            }else{
	      // allocated_list = NULL;
	      prev->next = NULL;
            }
	    free(currentNode);
          
            return 0;
            
        }else{
            prev = currentNode;
            currentNode = currentNode->next;//increment pointer
        }
        
    }
    
    return -1; //unsuccessful sfree();s
}


/*
 ============================================================================
 Helper Function for sfree() 
 - compares address's of LL nodes
 - inserts a block in linkedlist by increasing address order.
 ============================================================================
 */
struct block *insertOrdered(struct block *list, struct block *dest){

   struct block *curr = freelist , *prev = freelist;
   // struct block ** a = &dest;
   // (void) a;
   struct block *newNode = malloc(sizeof(struct block));
            if(newNode == NULL) {
                 perror("Malloc Failed");
                 exit(1);
            }
    
    newNode->addr = list->addr; //address of the node you are inserting
    newNode->size = list->size;
    assert(curr); //always have one free node
 
    /*Traverse linked list to compare memory addresses to find point of insertion */
    while(curr->next != NULL && newNode->addr >  curr->addr){
       prev = curr;
       curr = curr->next;
     }
    if(curr == prev){ /*insert to head */
       newNode->next = curr;
       freelist = newNode;
       return NULL;
     }
     if(curr->next == NULL){ /*insert to tail*/
	prev->next = newNode;
        newNode->next = curr;
	curr->next = NULL;
      }
     else{  /*insert between  internal node */
	prev->next = newNode;
        newNode->next = curr;        
      }
       
    return NULL;

  
}

