#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "smalloc.h"


#define SIZE 2096 


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
    
    printf("List of allocated blocks:\n");
    print_allocated();
    printf("List of free blocks:\n");
    print_free();
    printf("Contents of allocated memory:\n");
    print_mem();

    // Now call smalloc() after sfree() has been called
    for(i = 0; i < 4; i++) {
        int num_bytes = (i+1) * 100;
    
        ptrs[i] = smalloc(num_bytes);
        write_to_mem(num_bytes, ptrs[i], i);
    }
    printf("List of allocated blocks after sfree():\n");
    print_allocated();
    printf("List of free blocks:\n");
    print_free();
    printf("Contents of allocated memory:\n");
    print_mem();
    return 0;



    //char *ptrs[1];
    //int i;


    /* Call smalloc once to test single node case */
    
    // for(i = 0; i < 1; i++) {
    //     int num_bytes = (i+1) * 10;
    
    //     ptrs[i] = smalloc(num_bytes);
    //     write_to_mem(num_bytes, ptrs[i], i);
    // }
    // printf("List of allocated blocks:\n");
    // print_allocated();
    // printf("List of free blocks:\n");
    // print_free();
    // printf("Contents of allocated memory:\n");
    // print_mem();
    
    // printf("freeing %p result = %d\n", ptrs[0], sfree(ptrs[0]));
    
    // printf("List of allocated blocks:\n");
    // print_allocated();
    // printf("List of free blocks:\n");
    // print_free();
    // printf("Contents of allocated memory:\n");
    // print_mem();

    // mem_clean();
    // return 0;


}
