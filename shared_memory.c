
#include "shared_memory.h"



int SharedCreate(key_t key){

    if(key<0){
      printf("Error in key! /n");
      return -1;
    }

    return shmget(key, sizeof(data), IPC_CREAT | 0666);
}

data* SharedAttach(int SharedID){
    return shmat(SharedID, (void *)0, 0);
}
int SharedDetach(data* ShmPtr){
    return shmdt(ShmPtr);
}

int SharedDelete(int SharedID){
    return shmctl(SharedID, IPC_RMID, 0);
}