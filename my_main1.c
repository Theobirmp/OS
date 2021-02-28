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
    int sem_enc2;
    int sem_p2;


    //create the semaphores we will need
    //sem_p1 starts at 1 cause p1 is the first process that will take place
    sem_p1=SemCreate((key_t)45655,1,1);
    sem_enc1=SemCreate((key_t)45656,1,0);
    sem_chan=SemCreate((key_t)45657,1,0);
    sem_enc2=SemCreate((key_t)45658,1,0);
    sem_p2=SemCreate((key_t)45659,1,0);




    int Shm_p1_id;		//arguments for shared memory creation (enc1)
    data *Shm_p1_ptr;
    key_t key_p1=1034;
    Shm_p1_id = SharedCreate(key_p1);		//chech this functions at shared_memory.c
   	Shm_p1_ptr = SharedAttach(Shm_p1_id); 

    int Shm_enc1_id;		//arguments for shared memory creation (enc1)
    data *Shm_enc1_ptr;
    key_t key_enc1=1035;
    Shm_enc1_id = SharedCreate(key_enc1);		//chech this functions at shared_memory.c
   	Shm_enc1_ptr = SharedAttach(Shm_enc1_id);   //chech this functions at shared_memory.c

    int Shm_chan_id;		//arguments for shared memory creation (enc1)
    data *Shm_chan_ptr;
    key_t key_chan=1036;
    Shm_chan_id = SharedCreate(key_chan);		//chech this functions at shared_memory.c
   	Shm_chan_ptr = SharedAttach(Shm_chan_id);

    int Shm_enc2_id;		//arguments for shared memory creation (enc1)
    data *Shm_enc2_ptr;
    key_t key_enc2=1037;
    Shm_enc2_id = SharedCreate(key_enc2);		//chech this functions at shared_memory.c
   	Shm_enc2_ptr = SharedAttach(Shm_enc2_id);

    int Shm_p2_id;		//arguments for shared memory creation (enc1)
    data *Shm_p2_ptr;
    key_t key_p2=1038;
    Shm_p2_id = SharedCreate(key_p2);		//chech this functions at shared_memory.c
   	Shm_p2_ptr = SharedAttach(Shm_p2_id);

    //Shm_p1_ptr->p1_p2=1;
    int running=1;

    //used to read input from the user
    char str[30];
    //we set the flow of the programme from p1 to p2
    Shm_p1_ptr->p1_p2=1;
    Shm_enc2_ptr->p1_p2=1;
    Shm_chan_ptr->p1_p2=1;
    Shm_enc1_ptr->p1_p2=1;
    Shm_p2_ptr->p1_p2=1;
    
    //we use this variable to detect when the programme needs to stop running
    Shm_p1_ptr->running=1;
    Shm_enc1_ptr->running=1;
    Shm_chan_ptr->running=1;
    Shm_enc2_ptr->running=1;
    Shm_p2_ptr->running=1;
    
    while(Shm_p1_ptr->running==1){//the programme is terminated when the user types TERM

        SemDown(sem_p1,0);  //we are in process p1 
        if(Shm_p1_ptr->running==0){
            break;
    }
        
        if(Shm_p1_ptr->p1_p2==1){
        printf("P1 -> P2 \n");
        printf("P1: Enter a text :\n");
        fgets(str,25,stdin);

        memcpy(Shm_enc1_ptr->id,str,sizeof(str)); 
        SemUp(sem_enc1,0);  //now process 2 will start running
        
        }else if(Shm_p1_ptr->p1_p2==2){//we are in p1 but p2 send a message
            printf("this is the message that is transported %s\n",Shm_p1_ptr->id);
        
        if(Shm_p1_ptr->id[0]=='T' && Shm_p1_ptr->id[1]=='E' && Shm_p1_ptr->id[2]=='R' && Shm_p1_ptr->id[3]=='M' ){
                Shm_p1_ptr->running=0;  //we terminate the programme when the user types TERM
                Shm_enc1_ptr->running=0;
                Shm_chan_ptr->running=0;
                Shm_enc2_ptr->running=0;
                Shm_p2_ptr->running=0;
                SemUp(sem_enc1,0);  //all processes will end 
                SemUp(sem_chan,0);
                SemUp(sem_enc2,0);
                SemUp(sem_p2,0);
            }
            if(Shm_p1_ptr->running==0){
                break;
            }
            if(Shm_p1_ptr->running!=0){ //if the programme is not to be terminated we read next input
            printf("P1: Enter a text :\n");
            fgets(str,25,stdin);    //could be more than 25 but less than 30
        
            memcpy(Shm_enc1_ptr->id,str,sizeof(str));

            Shm_p1_ptr->p1_p2=1;
            Shm_enc2_ptr->p1_p2=1;
            Shm_chan_ptr->p1_p2=1;
            Shm_enc1_ptr->p1_p2=1;
            Shm_p2_ptr->p1_p2=1;
            SemUp(sem_enc1,0); 

        //}
    

}
    }
    }

    //delete everything after we are done
    //each process deletes its own things 
    SemDel(sem_p1);

    SharedDetach(Shm_p1_ptr);
    SharedDelete(Shm_p1_id);



   return 0;

}