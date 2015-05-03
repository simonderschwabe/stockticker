#ifndef NN_FILE_API_H
#define NN_FILE_API_H

#define PIXEL_X 3
#define PIXEL_Y 3
#define OUTPUT_NUMBER 1
int NN_TRAINING_ROWS = 0;

FILE *create_training_file(char*);
void close_training_file(FILE*);
int write_training_record(FILE*, int*, int*);
void finish_training_file(char*,char*);
int *generate_random_binary_array(int);

#endif
