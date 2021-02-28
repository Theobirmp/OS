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

    int sem_p1;
    int sem_enc1;
    int sem_chan;
    int sem_p2;

    //create the semaphores we will need
    //check the other files to understand what these functions do
    sem_p1=SemCreate((key_t)45655,1,1);
    sem_enc1=SemCreate((key_t)45656,1,0);
    sem_chan=SemCreate((key_t)45657,1,0);
    sem_p2=SemCreate((key_t)45659,1,0);


    int Shm_p1_id;		//arguments for shared memory creation (enc1)
    data *Shm_p1_ptr;
    key_t key_p1=1034;
    Shm_p1_id = SharedCreate(key_p1);		//chech this functions at shared_memory.c
   	Shm_p1_ptr = SharedAttach(Shm_p1_id);   //chech this functions at shared_memory.c
    

    int Shm_enc1_id;		//arguments for shared memory creation (enc1)
    data *Shm_enc1_ptr;
    key_t key_enc1=1035;
    Shm_enc1_id = SharedCreate(key_enc1);		//chech this functions at shared_memory.c
   	Shm_enc1_ptr = SharedAttach(Shm_enc1_id);   //chech this functions at shared_memory.c
    char str[100];//used to transfer the checksum


    int Shm_chan_id;		//arguments for shared memory creation (enc1)
    data *Shm_chan_ptr;
    key_t key_chan=1036;
    Shm_chan_id = SharedCreate(key_chan);		//chech this functions at shared_memory.c
   	Shm_chan_ptr = SharedAttach(Shm_chan_id);   //chech this functions at shared_memory.c
    int number=1;//used when p2 sends a message

    Shm_enc1_ptr->p1_p2=1;  //we set if for chan to do its thing when p1 sends a message




    while(Shm_enc1_ptr->running){
        SemDown(sem_enc1,0);
        if(Shm_enc1_ptr->running==0){
            break;
    }
        int altered=0;
        if(Shm_enc1_ptr->p1_p2==1){
            printf("enc1 -> P2 \n");
            //compute the checksum check other files to see that the function does
            compute_md5(Shm_enc1_ptr->id,str);
            //we transport the message to the next shared memory
            memcpy(Shm_chan_ptr->id,Shm_enc1_ptr->id,sizeof(Shm_enc1_ptr->id));
            memcpy(Shm_chan_ptr->checksum,str,sizeof(str));
            Shm_chan_ptr->p1_p2=1;
            //up the next semaphore in line so that the next process starts
            SemUp(sem_chan,0);
            //we just print sth for the examiner to see xD
            printf("this is the end of enc1\n");
        }else if(Shm_enc1_ptr->p1_p2==2){
            printf("enc1 -> P1 \n");
            for(int count=0;count<sizeof(Shm_enc1_ptr->id);count++){
            if(Shm_enc1_ptr->id[count]=='X'){
                altered=1;
            }
        }
        if(altered==1){
                altered=0;//reset it to get the new message
                printf("this message has been subject to noise so it is canceled\n");
                SemUp(sem_p2,0);
                //message is not send to p2 and process p1 asks for a new one
            }else{
                printf("this message didnt get chnanged\n");
                memcpy(Shm_p1_ptr->id,Shm_enc1_ptr->id,sizeof(Shm_enc1_ptr->id));
                Shm_p1_ptr->p1_p2=2;
                SemUp(sem_p1,0);
        }


    

    }
    Shm_p1_ptr->running=1;
    Shm_enc1_ptr->running=1;
    Shm_chan_ptr->running=1;
    
}
    //delete everything after we are done
    //each process deletes its own things 
    SemDel(sem_enc1);

    SharedDetach(Shm_enc1_ptr);
    SharedDelete(Shm_enc1_id);

    
    return 0;
}