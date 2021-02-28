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
    int sem_enc2;
    int sem_p2;
    int sem_chan;

    sem_p1=SemCreate((key_t)45655,1,1);
    sem_chan=SemCreate((key_t)45657,1,0);
    sem_enc2=SemCreate((key_t)45658,1,0);
    sem_p2=SemCreate((key_t)45659,1,0);

    int Shm_chan_id;		//arguments for shared memory creation (enc1)
    data *Shm_chan_ptr;
    key_t key_chan=1036;
    Shm_chan_id = SharedCreate(key_chan);		//chech this functions at shared_memory.c
   	Shm_chan_ptr = SharedAttach(Shm_chan_id);   //chech this functions at shared_memory.c

    int Shm_enc2_id;		//arguments for shared memory creation (enc1)
    data *Shm_enc2_ptr;
    key_t key_enc2=1037;
    Shm_enc2_id = SharedCreate(key_enc2);		//chech this functions at shared_memory.c
   	Shm_enc2_ptr = SharedAttach(Shm_enc2_id);   //chech this functions at shared_memory.c
    

    int Shm_p2_id;		//arguments for shared memory creation (enc1)
    data *Shm_p2_ptr;
    key_t key_p2=1038;
    Shm_p2_id = SharedCreate(key_p2);		//chech this functions at shared_memory.c
   	Shm_p2_ptr = SharedAttach(Shm_p2_id);   //chech this functions at shared_memory.c
    

    
    char str[100];//used to transfer the checksum
    Shm_enc2_ptr->p1_p2=1;
    while(Shm_enc2_ptr->running==1){

        SemDown(sem_enc2,0);
        if(Shm_enc2_ptr->running==0){
            break;
    }
        int altered=0;
        if(Shm_enc2_ptr->p1_p2==1){//the flow is from p1 to p2
        printf("enc2 -> P2 \n");
        for(int count=0;count<sizeof(Shm_enc2_ptr->id);count++){
            if(Shm_enc2_ptr->id[count]=='X'){
                altered=1;
            }
        }
            if(altered==1){
                altered=0;//reset it to get the new message
                printf("this message has been subject to noise so it is canceled\n");
                SemUp(sem_p1,0);
                
                //message is not send to p2 and process p1 asks for a new one
            }else{
                //this messged dididnt get changed
                printf("this message didnt get chnanged\n");
                memcpy(Shm_p2_ptr->id,Shm_enc2_ptr->id,sizeof(Shm_enc2_ptr->id));
                //this is enc2 btw
                printf("this is the final message unchanged  %s \n",Shm_enc2_ptr->id);
                SemUp(sem_p2,0);
        }

        }else if(Shm_enc2_ptr->p1_p2==2){//the flow is from p2 to p1
            printf("enc2 -> P1 \n");
            //we compute the checksum and transport the message
            compute_md5(Shm_enc2_ptr->id,str);
            memcpy(Shm_chan_ptr->id,Shm_enc2_ptr->id,sizeof(Shm_enc2_ptr->id));
            memcpy(Shm_chan_ptr->checksum,str,sizeof(str));
            Shm_chan_ptr->p1_p2=2;
            SemUp(sem_chan,0);
            Shm_chan_ptr->running=1;
            
            
        }
        //used to terminate the process
        if(Shm_enc2_ptr->running==0){
            break;
    }

    }
    //delete everything
    //each process deletes its own things 
    SemDel(sem_enc2);
    SharedDetach(Shm_enc2_ptr);
    SharedDelete(Shm_enc2_id);


    return 0;
}