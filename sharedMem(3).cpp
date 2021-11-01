#include <sys/ipc.h>
#include <semaphore.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include <fcntl.h>
#include <iostream>

#define RWRR (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

using namespace std;

sem_t call_sem_init( );
sem_t call_sem_open( );

// generate a key
key_t getIpcKey() {
	return ftok( "/home/tomabot/local/ipc/ipc.key", 'a' );

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
		"/tmp",		// name: the name of the semaphore
		O_CREAT|O_EXCL,	// oflag: create a new named semaphore if it doesn't exist yet
				// if it already exists, return an error
		RWRR,		// mode: like file permissions, owner rw, group r, other r
		1		// initial value of the semaphore
	);

	// ...debugging... sem_open returns a pointer to the semaphore if OK, SEM_FAILED on error
	cout << "sem_t=" << (void *) psem << endl;
	return *psem;
}

// create an unnamed POSIX Semaphore to be used between related processes
sem_t call_sem_init( ) {
	sem_t sem;

	int rtnValue = sem_init( 
		&sem,	// address of semabore variable
		1, 	// using semaphore with multiple processes, use nonzero
		1 	// initial value of the semaphore
	);
	cout << "sem_init() returned " << rtnValue << endl;
	return sem;
}

// this function returns a shared memory identifier
int getShmId() {
	const size_t shmSize = 1024;
	const int shmFlags = IPC_CREAT;
	key_t ipcKey = getIpcKey();

	int shmID = shmget( ipcKey, shmSize, shmFlags );
	
	// ...debugging... show the shared memory ID returned by shmget()
	cout << "shmID:" << shmID << endl;
	return shmID;
}

int main() {
	sem_t sem = call_sem_open();
	int shmId = getShmId();
	return( 0 );
}


