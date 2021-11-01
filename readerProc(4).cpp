
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

#include <sys/ipc.h>
#include <semaphore.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include <fcntl.h>

using namespace std;

#define RWRR (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

static void sig_usr( int );	// signal handler

const size_t shmSize = 1024;
char *sharedData;
sem_t *psem;

int main( void ) {
	const int shmFlags = IPC_CREAT;
	int shmId;
	key_t ipcKey;

	pid_t pid = getpid();

	// set up a SIGUSR1 signal handler
	if( signal( SIGUSR1, sig_usr ) == SIG_ERR ) {
		printf( "can't catch SIGUSR1\n" );
	}

	// create a named POSIX Semaphore
	sem_unlink( "/tmp" );
	psem = sem_open(
		"/tmp",		// the name of the semaphore
		O_CREAT | O_EXCL,	// create a new named semaphore if it doesn't exist yet
				// if it already exists, return an error
		RWRR,		// owner rw, group r, other r
		1		// initial value of the semaphore
	);


	// create a shared memory segment
	ipcKey = ftok( "/home/tomabot/ipc.key", 'a' );

	shmId = shmget( ipcKey, shmSize, shmFlags );
	
	// attach to the segment, to get a pointer to it
	sharedData = (char *) shmat(shmId, NULL, 0);

	// infinite loop showing process id and a "tick" message
	for( ; ; ) {
		sleep( 1 );
		printf( "...tick(%d)...\n", pid );
	}
}

static void sig_usr( int signo ) {
	if( signo == SIGUSR1 ) {
		printf( "...zap..." );
	}

	// acquire the semaphore
	
	// display contents on standard out
	printf( "%s\n", sharedData );

	// clear the shared memory segment
	for( int i = 0; i < shmSize; i++ ) {
		shmSize[i] = "\0";
	}

	// release the semaphore

}

