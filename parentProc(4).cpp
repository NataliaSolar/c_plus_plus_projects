#include <iostream> 
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <semaphore.h>
#include <string>
#include <cstring>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <fcntl.h>

#define RWRR (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

using namespace std; 

//Roman Sobolenko
//CSD415 Assignment 
//Inter process communication through shared memory.
//parentProc ismywriter - takes input from user



//1. Start create, attach to shared memory
//2. create semaphore
//3. Fork and start child process
//4. Loop prompting the user for input
//5. In that loop aquire semphare and write usr inout to shared memory
//6. Release semaphore

sem_t *sem;
  
int main() 
{ 
    // ftok to generate unique key 
    key_t key = ftok("shmfile",65); 
  
    // shmget returns an identifier in shmid 
    int shmid = shmget(key,1024,0666|IPC_CREAT); 
  
    // shmat to attach to shared memory 
    char *myStr = (char*) shmat(shmid,(void*)0,0); 


    char * mySem = "/tmpsen";
    sem_unlink(mySem);                              //destroy sem if exists
    sem = sem_open(mySem, O_CREAT|O_EXCL, RWRR, 1); //create sem
  

    //fork to the child
    if(fork() == 0){
        //use gnome to open another terminal
       // string s = "gnome-terminal -- ./childProc";

        system("gnome-terminal -- ./childProc"); 


    }

    else{
        while(true){


    //array to store user input
    char *userInput = new char[256];

    cout<<"\nWrite Data : ";
    cin >> userInput;
   // cin >> myStr;     //consolue_input 


    //aquire sempaphore from the user input
    sem_wait(sem); // wait for sem

    //add user input to shared mem
    strcat(myStr, userInput);
    strcat(myStr, "\n");

    //realease the semaphore
    sem_post(sem); 

  
    printf("Data written in memory: %s\n",myStr); 
        printf(userInput);
        }
        //detach from shared memory  
        shmdt(myStr); 

    }

    return 0; 
        
} 