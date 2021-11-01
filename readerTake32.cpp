
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


using namespace std;

#define RWRR (S_IRWXU | S_IRGRP | S_IROTH)
#define INTVAL 1

sem_t *sem;
int shmid;
key_t key;
char *shm_buffer;

void sig_usr1( int signo ) {
	if( signo != SIGUSR1 ) {
		printf("...oops...\n");
	}

	// decrement (or lock) the semaphore (using sem_wait)
	sem_wait( sem );
	
	// display contents of shared memory segment
	printf("write process says: %s\n", shm_buffer);
	
	// clear contents of shared memory
	for( int i = 0; i < 1024; i++ ) {
		shm_buffer[i] = '\0';
	}
	
	// release semaphore (using sem_post)
	sem_post( sem );
}

int main() {
	// get pid
	pid_t pid = getpid();

	// setup signal handler
	if( signal(SIGUSR1, sig_usr1) == SIG_ERR) {
		printf(" can't catch siguser1\n");
		exit(-1);
	}
	
	// create a named semaphore
	sem_unlink( "/tmpsem" );	// just in case there's one, delete it
	sem = sem_open("/tmpsem", O_CREAT|O_EXCL, RWRR, INTVAL);
	
	// create a named shared memory segment
	key = ftok( "/home/julie/ipc.key", 'a' );
	shmid = shmget(key, 1024, 0666 |  IPC_CREAT);
	
	// attach to the named shared memory segment
	shm_buffer = (char *) shmat(shmid, (void *) 0, 0);
	
	// loop for-ever
	for(;;) {
		printf("...tick...(%d)...\n", pid);
		sleep( 1 );
	}
}


