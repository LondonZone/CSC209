#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "smalloc.h"


#define SIZE 4096 


/* Complicated test for smalloc and sfree. */

int main(void) {

    mem_init(SIZE);

    char *ptrs[10];
    int i;

    /* Call smalloc 4 times */

   for(i = 0; i < 4; i++) {
        int num_bytes = (i+1) * 10;
    
        ptrs[i] = smalloc(num_bytes);
        write_to_mem(num_bytes, ptrs[i], i);
    }


    printf("List of allocated blocks:\n");
    print_allocated();
    printf("List of free blocks:\n");
    print_free();
    printf("Contents of allocated memory:\n");
    print_mem();
    
    printf("freeing %p result = %d\n", ptrs[1], sfree(ptrs[1]));
    printf("freeing %p result = %d\n", ptrs[2], sfree(ptrs[2]));

    printf("List of allocated blocks:\n");
    print_allocated();
    printf("List of free blocks:\n");
    print_free();
    printf("Contents of allocated memory:\n");
    print_mem();
    mem_clean();
    return 0;
    
    //Now call smalloc() after sfree() has been called
    // for(i = 0; i < 4; i++) {
    //     int num_bytes = (i+40);
    
    //     ptrs[i] = smalloc(num_bytes);
    //     write_to_mem(num_bytes, ptrs[i], i);
    // }
    // printf("List of allocated blocks after sfree():\n");
    // print_allocated();
    // printf("List of free blocks:\n");
    // print_free();
    // printf("Contents of allocated memory:\n");
    // print_mem();
    
    
    
    
    
    
    /* Call smalloc once to test single node case 
    char *ptrs2[1];
    int j;
    printf("\n=======================\n");
    printf("Single block test case:\n");
    printf("=======================\n\n");
    for(j = 0; j < 1; j++) {
        int num_bytes = (i+1) * 10;
    
        ptrs2[j] = smalloc(num_bytes);
        write_to_mem(num_bytes, ptrs2[j], j);
    }
    printf("List of allocated blocks:\n");
    print_allocated();
    printf("List of free blocks:\n");
    print_free();
    printf("Contents of allocated memory:\n");
    print_mem();
    
    printf("freeing %p result = %d\n", ptrs2[0], sfree(ptrs2[0]));
    
    printf("List of allocated blocks:\n");
    print_allocated();
    printf("List of free blocks:\n");
    print_free();
    printf("Contents of allocated memory:\n");
    print_mem();

    mem_clean();
    return 0;

    */
    //printf("\n==================================\n");
    //printf("Call Smalloc() 4 times then sfree():\n");
    //printf("====================================\n\n");
    
    



}
