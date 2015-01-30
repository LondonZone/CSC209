/*
 ============================================================================
 Name        : addecho.c
 Author      : David London
 Version     :
 Copyright   : Your copyright notice
 Description : CSC209 Assignment 1 - removes vocals from stereo sound files
 ============================================================================
 */
/**
 *
 *
 * Copy the first 44 bytes from the input file to the output file
 * without changing those bytes.
 * Those 44 bytes contain important header information
 * that should not be modified by your program.
 * Next, treat the rest of the input file as a sequence of shorts.
 * Take each pair of shorts left and right, and compute combined = (left - right) / 2
 * Write two copies of combined to the output file.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#define HEADER_SIZE 22
// you need 3 arrays
// one is current one is echoed and one is mixed
// mix them onto the mixed array
//then add the current stuff on the echod one
int computeEcho(short current[], short echoBuffer[], short mixed[], int volume_scale, int delay){
		int i;
		for(i=0; i < delay; i++){
		mixed[i] = current[i] + (echoBuffer[i]/volume_scale);
	
	  
		}
		return i;
		//keep track of remaining echoBuffer
	
}
	
void setEcho(short echoBuffer[], short current[], int delay){
	for (int i = 0; i < delay; i++) {
		echoBuffer[i] = current[i];

	}
	
}

void setSilence(short current[], int delay, int x){
	int difference = delay - x;
	for (int i = 0; i < difference; i++) {
		current[delay - difference + i] = 0;

	}
	
}


int main(int argc, char *argv[]){
	/**
	* If -d is not provided, use a default value of 8000;
	* if -v is not provided, use a default value of 4.
	*
	*/
	int option = 0;
	
	/*Default values for delay and volume*/
	int delay = 8000;
	int volume_scale = 4;
	
	while ((option = getopt(argc, argv, "d:v:")) != EOF)  {
		switch(option) {
					case 'd':
						delay = atoi(optarg);
						break;

					case 'v':
						volume_scale = atoi(optarg);
						break;
					default:
						break;
				}
	//printf("%d\n",atoi(optarg));
	}
	if (optind >= argc) {
	        fprintf(stderr, "Expected argument after options\n");
	        exit(EXIT_FAILURE);
	    }
	

	printf("%d\n%d\n",delay,volume_scale);
	
	FILE *fp, *fpDest;

	// input file, returns filestream pointer 
	fp = fopen(argv[argc-2],"rb"); 
	if (fp == NULL){
			fputs ("\n fopen() error.",stderr);
		    exit (1);
	}
	// Destination/output File
	fpDest = fopen(argv[argc-1],"wb");


	short header[HEADER_SIZE];	
	fread(header, sizeof(short), 22, fp);
	
	//pointer to a memory address which contains an unsigned int
	unsigned int *sizeptr = (unsigned int *)(header + 2); 
	*sizeptr += (delay * 2);
	
	sizeptr = (unsigned int *)(header + 20); 
	*sizeptr += (delay * 2);
	
	//write the header
	fwrite(header, sizeof(short), 22, fpDest);
	
	
	//short *Buffer = malloc(sizeof(delay));
	short current[delay];
	short echoBuffer[delay];
	//short mixed[delay];
	
	//read and write samples before delay
	fread(echoBuffer, sizeof(short), delay, fp);
	fwrite(echoBuffer, sizeof(short), delay, fpDest);
	
	int i;
	int x;
 	while(!feof(fp)) {
		//read samples once delay is reached
		x = fread(current, sizeof(short), delay, fp);
	
		
		/*2) start mixing in samples from the echobuffer */
		for (i=0; i < delay; i++) {
			short mixed = current[0] + (echoBuffer[i]/volume_scale);
			fwrite(&mixed, sizeof(short), delay, fpDest); //up to index zeros
		}
		//position = computeEcho(current, echoBuffer, mixed, volume_scale, delay);
		//write echobuffer before writing mix
		
		
		setEcho(echoBuffer, current, delay);
		setSilence(current, delay, x);
		i++;

	}
	printf("%d",x);
	/*	3. adding the remaining echo */
	int k;
	for(k=0; k < x; k++) {
		//scaleEcho(echoBuffer, volume_scale, delay-1);
		short temp = (echoBuffer[x]/volume_scale);
		fwrite(&temp, sizeof(short), 1, fpDest);
		
	}
	
					

	
					
	return 0;


	
}
