#include <sys/wait.h>
#include <unistd.h>
#include <sys/timeb.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "semaphores.h"
#include "shared_memory.h"



int main (int argc, char* argv[]){
    srand(time(NULL));//used to create the random number

    int sem_enc1;
    int sem_chan;
    int sem_enc2;

    sem_enc1=SemCreate((key_t)45656,1,0);
    sem_chan=SemCreate((key_t)45657,1,0);
    sem_enc2=SemCreate((key_t)45658,1,0);

    int Shm_enc1_id;		//arguments for shared memory creation (enc1)
    data *Shm_enc1_ptr;
    key_t key_enc1=1035;
    Shm_enc1_id = SharedCreate(key_enc1);		//chech this functions at shared_memory.c
   	Shm_enc1_ptr = SharedAttach(Shm_enc1_id);   //chech this functions at shared_memory.c

    int Shm_chan_id;		//arguments for shared memory creation (enc1)
    data *Shm_chan_ptr;
    key_t key_chan=1036;
    Shm_chan_id = SharedCreate(key_chan);		//chech this functions at shared_memory.c
   	Shm_chan_ptr = SharedAttach(Shm_chan_id);   //chech this functions at shared_memory.c
    char str[100];//used to transfer the checksum


    int Shm_enc2_id;		//arguments for shared memory creation (enc1)
    data *Shm_enc2_ptr;
    key_t key_enc2=1037;
    Shm_enc2_id = SharedCreate(key_enc2);		//chech this functions at shared_memory.c
   	Shm_enc2_ptr = SharedAttach(Shm_enc2_id);   //chech this functions at shared_memory.c
    
    int number=1;
    int random_number;
    int argv_number;


    Shm_chan_ptr->p1_p2=1;
    while(Shm_chan_ptr->running==1){
        
        SemDown(sem_chan,0);
    //     if(Shm_chan_ptr->running==0){
    //         break;
    // }
        
        if(Shm_chan_ptr->p1_p2==1){
        printf("chan -> P2 \n");
        //the argument we passed in
        //PLS check readme for more info
        argv_number=atoi(argv[1]);
        
        for(int count=0;count<sizeof(Shm_chan_ptr->id);count++){
            random_number = rand() % 100; ///random number between [0,99]
            if(random_number<argv_number){ //we need to change this letter
                Shm_chan_ptr->id[count]='X';    

            }
        }
        //just a random message for the examiner
        printf("all good till here \n");
        //we pass the message to the next process
        memcpy(Shm_enc2_ptr->id,Shm_chan_ptr->id,sizeof(Shm_chan_ptr->id));
        SemUp(sem_enc2,0);
        printf("this is the end of chan\n");
    }else if(Shm_chan_ptr->p1_p2==2){
        printf("chan - > P1 \n");
        for(int count=0;count<sizeof(Shm_chan_ptr->id);count++){
            random_number = rand() % 100; ///random number between [0,99]
            argv_number=atoi(argv[1]);
            if(random_number<argv_number){ //we need to change this letter
                Shm_chan_ptr->id[count]='X';    

            }
        }
        
        memcpy(Shm_enc1_ptr->id,Shm_chan_ptr->id,sizeof(Shm_chan_ptr->id));
        Shm_enc1_ptr->p1_p2=2;
        SemUp(sem_enc1,0);
        
    }
    if(Shm_chan_ptr->running==0){
        break;


    }
}
    //delete everything we need to
    //each process deletes its own things 
    SemDel(sem_chan);
    SharedDetach(Shm_chan_ptr);
    SharedDelete(Shm_chan_id);

    return 0;
}