#include <stdio.h>
#include <stdlib.h>
#include "nn_file_api.h"

/*
#define PIXEL_X 3
#define PIXEL_Y 3
#define OUTPUT_NUMBER 1
int NN_TRAINING_ROWS = 0;

FILE *create_training_file(char*);
void close_training_file(FILE*);
int write_training_record(FILE*, int*, int*);
void finish_training_file(char*,char*);
int *generate_random_binary_array(int);

int main(){

        FILE *tr;
        int output[2] = {1};
        int output2[2] = {0};
        int *inp;
        int x;

        tr = create_training_file("testinput.txt");

        for(x = 0; x < PIXEL_X * PIXEL_Y; x++){

        inp = generate_random_binary_array(16);
        write_training_record(tr,inp,output);
        inp = generate_random_binary_array(16);
        write_training_record(tr,inp,output2);

        }

        close_training_file(tr);

        finish_training_file("testinput.txt","temp.txt");

        return 0;
}
*/

/***********************************************************
 * Name:
 *      create_training_file
 * Description:
 *      Creates an empty file. Overwrites existing file.
 * Parameter:
 *      1.) Filename
 * ReturnValue:
 *      1.) Pointer of Type FILE
 **********************************************************/
FILE *create_training_file(char *filename){

        FILE *training_file;

        training_file = fopen(filename, "w+");

        return training_file;
}

/***********************************************************
 * Name:
 *      close_training_file
 * Description:
 *      Closes open Filepointer
 * Parameter:
 *      1.) Pointer of Type FILE
 * ReturnValue:
 **********************************************************/
void close_training_file(FILE *fp){

        fclose(fp);

}

/***********************************************************
 * Name:
 *      write_training_record
 * Description:
 *      Writes a row with Input Neurons and a row
 *      with Output Neurons to Training Datafile and
 *      Updates global Row counter
 * Parameter:
 *      1.) Pointer of Type FILE
 *      2.) Pointer to input Neuron Value Integer Array
 *      3.) Pointer to outpu Neuron Value Integer Array
 * ReturnValue:
 *      0 for Success
 *      1 for Failure
 **********************************************************/
int write_training_record(FILE *fp, int *input, int *output){

        int i;

        for(i=0; i<PIXEL_X*PIXEL_Y; i++)        fprintf(fp,"%i ",input[i]);

        fprintf(fp,"\n");

        for(i=0; i<OUTPUT_NUMBER; i++)          fprintf(fp,"%i ",output[i]);

        fprintf(fp,"\n");

        NN_TRAINING_ROWS++;

        return 0;
}

/***********************************************************
 * Name:
 *      finish_training_file
 * Description:
 *      Adds the number of Rows and the Input and Output Neuron
 *      Number to the beginning of the file
 * Parameter:
 *      1.) Filename
 *      2.) Temporary Filename -> will be remove automaticaly
 * ReturnValue:
 **********************************************************/
void finish_training_file(char *filename, char *filenametemp){

        FILE *src;
        FILE *target;
        char ch;

        src     = fopen(filename,"r");
        target  = fopen(filenametemp, "w+");

        fprintf(target,"%i %i %i\n", NN_TRAINING_ROWS, PIXEL_X*PIXEL_Y, OUTPUT_NUMBER);

        while( ( ch = fgetc(src) ) != EOF )
                        fputc(ch,target);

        fclose(src);
        fclose(target);

        remove(filename);
        rename(filenametemp,filename);
}

/***********************************************************
 * Name:
 *      generate_random_binary_array
 * Description:
 *      Initializes an Integer Array with random Binary Numbers
 * Parameter:
 *      1.) Size of the Array
 * ReturnValue:
 *      1.) Pointer to the newly created array
 **********************************************************/
int *generate_random_binary_array(int size){

        int *tmp;
        int i;

        tmp = (int*) malloc(sizeof(int) * size);

        for(i = 0; i < size; i++) tmp[i] = rand() % 2;

        return tmp;
}
