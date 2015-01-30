/*
 ============================================================================
 Name        : A1.c
 Author      : David London-Schacht
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

int main(int argc, char *argv[]){
	
		FILE *fp, *fpDest; // pointer to a file type
		
		
		short buff_head[44]; 
		short bufferLeft[1]; 
		short bufferRight[1];		
		
		
		// input file, returns filestream pointer 
		fp = fopen(argv[1],"rb"); 
		if (fp == NULL){
			fputs ("\n fopen() error.",stderr);
			exit (1);
		}
		
		// Destination/output File
		fpDest = fopen(argv[2],"wb");
		if(fpDest == NULL){
			fputs ("\n fopen() error.",stderr);
			exit (1);
		}
		
		//parse the header/first 44 bytes of Input file
		fread(&buff_head, sizeof(short), 44, fp);
		fwrite(&buff_head, 44 * sizeof(short), 1, fpDest);

		
			
		int computed;
				
		while (!feof(fp)) {
			/* shorts are read one at a time
				1) first short read is left
				2) second short read is right
			*/
			fread(&bufferLeft, sizeof(short), 1, fp);
			fread(&bufferRight,  sizeof(short), 1, fp);
		
							
			
			//Write two copies of combined to the output file.
			computed = (bufferLeft[0] - bufferRight[0])/2;
			fwrite(&computed, sizeof(short), 1, fpDest);
			fwrite(&computed, sizeof(short), 1, fpDest);

			
		}
			
		
		 fclose(fp);
		 fclose(fpDest);

}



