#include <iostream> 
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
using namespace std; 

//Natalia Solar
//CSD415 Assignment 3
//Inter process communication through shared memory.


//attach to shmem

//prompt user for some text

//acquire semaphore

//write text to shmem

//relese semaphore
  
int main() 
{ 
    // ftok to generate unique key 
    key_t key = ftok("shmfile",65); 
  
    // shmget returns an identifier in shmid 
    int shmid = shmget(key,1024,0666|IPC_CREAT); 
  
    // shmat to attach to shared memory 
    char *str = (char*) shmat(shmid,(void*)0,0); 
  
    cout<<"Write Data : "; 
    //gets(str); 
    //char * gets (str* );

  
    printf("Data written in memory: %s\n",str); 
      
    //detach from shared memory  
    shmdt(str); 
  
    return 0; 
} 