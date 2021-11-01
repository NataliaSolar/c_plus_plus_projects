#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

#include <sys/types.h>
#include <sys/mman.h>
#include <csignal>
#include <unistd.h>
#include <errno.h>

using namespace std;

#define INTVAL 1
#define ERR -1
#define SHMBUFFLEN 256

sem_t *sem;
int shmfd;
key_t key;
char *shm_buffer;

void sig_usr1( int signo ) {
	if( signo != SIGUSR1 ) {
		printf( "...oops...\n" );
	}

	// decrement the POSIX semaphore using sem_wait
	sem_wait( sem );
	
	// display contents of shared memory segment
	printf( "<%s>\n", shm_buffer );
	
	// clear contents of shared memory
	for( int i = 0; i < SHMBUFFLEN; i++ ) {
		*( shm_buffer + i )  = '\0';
	}
	
	// release semaphore (using sem_post)
	sem_post( sem );
}

int main( ) {
	// get pid
	pid_t pid = getpid( );

	// setup signal handler
	if( signal( SIGUSR1, sig_usr1 ) == SIG_ERR ) {
		printf( "%s\n", strerror( errno ));
		exit( ERR );
	}
	
	// just in case the POSIX semaphore is left over from a previous run
	sem_unlink( "/tmpsem" );

	// create a named POSIX semaphore
	if( SEM_FAILED == ( sem = sem_open( 
			"/tmpsem", 
			O_CREAT | O_EXCL, 
			S_IRWXU | S_IRGRP | S_IROTH, 
			INTVAL 
		))) {
		printf( "%s\n", strerror( errno ));
		exit( ERR );
	}

	// create a POSIX shared memory segment
	if( ERR == ( shmfd = shm_open( 
			"/tmpshm", 
			O_RDWR | O_CREAT,
			S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH
		))) {
		printf( "%s\n", strerror( errno ));
		exit( ERR );
	}
	
	// the newly created shared memory segment is 0, so
	// resize it using ftruncate
	if( ERR == ftruncate( shmfd, SHMBUFFLEN )) {
		printf( "%s\n", strerror( errno ));
		exit( ERR );
	}

	if( MAP_FAILED == ( shm_buffer = ( char * )  mmap ( 
			NULL, 
			SHMBUFFLEN, 
			PROT_READ | PROT_WRITE, 
			MAP_SHARED,
			shmfd,
			0 
		))) {
		printf( "%s\n", strerror( errno ));
		exit( ERR );
	}

	// loop for-ever
	for(;;) {
		printf( "...tick...(%d)...\n", pid );
		sleep( 1 );
	}
}

