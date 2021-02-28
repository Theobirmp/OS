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
    int sem_enc1;
    int sem_chan;
    int sem_p1;
    int sem_enc2;
    int sem_p2;


    sem_enc1=SemCreate((key_t)45656,1,0);
    sem_chan=SemCreate((key_t)45657,1,0);
    sem_p1=SemCreate((key_t)45655,1,1);
    sem_enc2=SemCreate((key_t)45658,1,0);
    sem_p2=SemCreate((key_t)45659,1,0);


    int Shm_p1_id;		//arguments for shared memory creation (enc1)
    data *Shm_p1_ptr;
    key_t key_p1=1034;
    Shm_p1_id = SharedCreate(key_p1);		//chech this functions at shared_memory.c
   	Shm_p1_ptr = SharedAttach(Shm_p1_id);   //chech this functions at shared_memory.c
    //char str[100];//used to transfer the checksum

    int Shm_enc1_id;		//arguments for shared memory creation (enc1)
    data *Shm_enc1_ptr;
    key_t key_enc1=1035;
    Shm_enc1_id = SharedCreate(key_enc1);		//chech this functions at shared_memory.c
   	Shm_enc1_ptr = SharedAttach(Shm_enc1_id);   //chech this functions at shared_memory.c
    //char str[100];//used to transfer the checksum

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
    

    char str[30];
    Shm_p2_ptr->p1_p2=1;

    while(Shm_p2_ptr->running==1){
        SemDown(sem_p2,0);
        //used to terminate the process
        if(Shm_p2_ptr->running==0){
            break;
    }
        if(Shm_p2_ptr->p1_p2==1){//flow is from p1 to p2
        
            printf("this is the message that is transported %s\n",Shm_p2_ptr->id);
            if(Shm_p2_ptr->id[0]=='T' && Shm_p2_ptr->id[1]=='E' && Shm_p2_ptr->id[2]=='R' && Shm_p2_ptr->id[3]=='M' ){
                //used to know when the programme needs to stop running
                Shm_p1_ptr->running=0;
                Shm_enc1_ptr->running=0;
                Shm_chan_ptr->running=0;
                Shm_enc2_ptr->running=0;
                Shm_p2_ptr->running=0;
                SemUp(sem_p1,0);
                SemUp(sem_enc1,0);
                SemUp(sem_chan,0);
                SemUp(sem_enc2,0);
                SemUp(sem_p2,0);
            }
            if(Shm_p2_ptr->running!=0){
            printf("P2: Enter a text :\n");
            fgets(str,25,stdin);
            //transport the message to the next process
            memcpy(Shm_enc2_ptr->id,str,sizeof(str));
            //the flow of the programme needs to change
            //message will be delivered from p2 to p1 now
            Shm_p1_ptr->p1_p2=2;
            Shm_enc2_ptr->p1_p2=2;
            Shm_chan_ptr->p1_p2=2;
            Shm_enc1_ptr->p1_p2=2;
            Shm_p2_ptr->p1_p2=2;
            //up the next semaphore which is sem_enc2 cause we are going from p2 to p1
            SemUp(sem_enc2,0);  
        }
              
        
    }else if(Shm_p2_ptr->p1_p2==2){
        //read input and transport the message
        printf("P2: Enter a text :\n");
        fgets(str,25,stdin);
        memcpy(Shm_enc2_ptr->id,str,sizeof(str)); 
        SemUp(sem_enc2,0);



    }
    }

    //delete everything
    //each process deletes its own things 
    SemDel(sem_p2);

    SharedDetach(Shm_p2_ptr);
    SharedDelete(Shm_p2_id);

    
    return 0;
}