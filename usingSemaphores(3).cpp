#include <sys/ipc.h>
#include <semaphore.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <iostream>

#define RWRR (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

using namespace std;

sem_t call_sem_init( );
sem_t call_sem_open( );

int main() {
	sem_t sem = call_sem_open();
	cout << "sem=" << (void *) &sem << endl;
	return( 0 );
}


// generate a
key_t getIpcKey() {
	return ftok( "/home/tomabot/local/ipc/ipc.key", 'a' );

}


// create a named POSIX Semaphore 
sem_t call_sem_open( ) {
	sem_t *psem = (sem_t *) 0;
	psem = sem_open(
		"/tmp",		// the name of the semaphore
		O_CREAT, //|O_EXCL,	// create a new named semaphore if it doesn't exist yet
				// if it already exists, return an error
		RWRR,		// owner rw, group r, other r
		1		// initial value of the semaphore
	);
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
