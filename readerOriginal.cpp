#include <iostream>
#include <csignal>
#include <unistd.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <fcntl.h>

//Natalia Solar
//CSD415 Assignment 3
//Inter process communication through shared memory.

		// acquire the semaphore

		// retrieve the contents of the shared mem segment

		// display the contents of the shared mem segment to stdout

		// clear the contents of the shared mem segment

using namespace std;

static void sig_usr( int );	// signal handler
#define RWRR (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

static void sig_usr( int signo ) {
	if( signo == SIGUSR1 ) {
		cout << "received SIGUSR1" << endl;



	} else {
		cerr << "...oops..." << endl;
	}
}

// Create a named POSIX Semaphore. When you are going to use an existing
// semaphore, you only specify two arguments: the name of the semaphore,
// and a zero value for the oflag argument
sem_t call_sem_open( ) {
        sem_t *psem = (sem_t *) 0;
        sem_unlink( "/tmp" );

        // the oflag parameter used in this call says to create a new semaphore.
        // // and if it already exists, cause sem_open to fail
        psem = sem_open(
                "/tmp",         // name: the name of the semaphore
                O_CREAT|O_EXCL, // oflag: create a new named semaphore if it doesn't exist yet
                                // if it already exists, return an error
                RWRR,           // mode: like file permissions, owner rw, group r, other r
                1               // initial value of the semaphore
        );

        // ...debugging... sem_open returns a pointer to the semaphore if OK, SEM_FAILED on error
        cout << "sem_t=" << (void *) psem << endl;
        return *psem;
}

// this function returns a shared memory identifier
int getShmId() {
        const size_t shmSize = 1024;
        const int shmFlags = IPC_CREAT;

	// generate an ipc key
        key_t ipcKey = ftok( "/home/user/ipc.key", 'a' );

	// create a shared memory segment
        int shmID = shmget( ipcKey, shmSize, shmFlags );

        // ...debugging... show the shared memory ID returned by shmget()
        cout << "shmID:" << shmID << endl;
        return shmID;
}

void sig_usr1(int signo ){
	
}

int main( void ) {
	// setup singal handler
	signal (SIGUSR1, sig_usr1);

	// create a named semaphore
	sem_t sem = call_sem_open();

	// create a shared mem segment
	int shmId = getShmId();

	// enter an endless loop 
	pid_t pid = getpid();
	//
	for( ;; ) {
		cout << "...tick " << pid << "..." << endl;
		sleep( 1 );
	}
	return(0);
}
