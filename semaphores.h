//Header file for Semaphores

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>

union semun{
    int val;
    struct semid_ds *buf;
    ushort *array;
};



int SemCreate(key_t,int,int); // Initialize semaphores

int SemDown(int,int); //Down the semaphore

int SemUp(int,int);  //Up the semaphore

int SemDel(int); // Delete the semaphore

void compute_md5(char *str, unsigned char digest[16]);