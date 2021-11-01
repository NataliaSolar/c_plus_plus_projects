#include <iostream> 
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <csignal>

//Natalia Solar
//CSD415 Assignment 
//Inter process communication through shared memory.
//childProc


	//1. The child is started by the parent
	//2. Connect to shared memory using cmd args that are passed in
	//3. Connect to the semaphore that was creared by the parent
	//4. start infinite loop
	//5.wait 10 - 15 seconds
	//6.take possion of semaphore
	//7.print the memory to the screan
	//8.clear the memory
	//9.realse semaphore


using namespace std;

#define RWRR (S_IRWXU | S_IRGRP | S_IROTH)
#define INTVAL 1

int shmid;
key_t key;
char *shm_buffer;

int main(int argc, char *argv[]) {
	
	//connect to semaphore
	sem_t *sem = sem_open("/tmpsen", 0);
	
	// create a named shared memory segment
	key = ftok("shmfile",65); 

	//Getting the ID of the shared memory
	int shmid = shmget(key, 1024, 0666|IPC_CREAT); 

	char*myStr = (char* )shmat(shmid, (void*)0,0);
	
	// loop for-ever
	for(;;) {
		sleep(5);		//sleep to wait for data
		sem_wait(sem);	//wait for and take possesion of semaphore
		cout << myStr; 	//print all the substrings
		*myStr = '\0'; 	//clear content of shared memory
		sem_post(sem);	//release semaphore
		//printf("...tick...(%d)...\n", pid);
	}
	return 0;
}