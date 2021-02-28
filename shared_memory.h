//Header file for Shared Memory

#pragma once

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>



typedef struct data{ //The definition of the data that will be used in the main program
    char id[30];
    char checksum[30];
    int p1_p2;
    int running;
}data;


int SharedCreate(key_t); //Creating shared memory 

data *SharedAttach(int); //Getting a pointer to the shared memory segment

int SharedDetach(data*); //Detaching the shared segment

int SharedDelete(int); //Deleting the shared memory segment