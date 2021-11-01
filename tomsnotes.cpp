/*
 * parent.cpp
 *
 * You start this program from a command line. When it starts up,
 * the first thing it does is call fork. 
 * 
 * The child process  immediately calls fork with a command that 
 * creates a new terminal window and runs the process ulc. The 
 * program ulc reads standard-in one character at a time. If the 
 * character just read is upper case, it is converted to lower
 * case. If the character is a new-line character, the output
 * buffer is flushed (making it appear on standard-out if it
 * already hasn't appeared). 
 *
 * The system() call executes a call to fork(). The child process
 * then calls exec with the string argument that was passed to 
 * system(). The parent process waits for the child process to 
 * finish. In other words, the parent process will block until
 * the child exits. 
 *
 * So this program calls system() from the child process created 
 * by the fork() call made from the if-statement in the code below.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>

using namespace std;

#define INPUTBUFFSZ 128
#define SHMBUFFLEN 1024
#define INTVAL 1
#include ERR -1

sem_t *sem;
int shmfd;0

char *shmBuff;
char inputBuff[INPUTBUFFSZ];

int main( void ) {
	// init stuff here...
	


	if( fork() == 0 ) {
		system( "gnome-terminal -- bash -c \"/home/child\"" );

	} else {

		// create the semaphore here
		// just in case the POSIX semaphore is left over from a previous run
		sem_unlink( "/tmpsem" );
		
		// create a named POSIX semaphore
		if( SEM_FAILED == ( sem = sem_open( 
				"/tmpsem", 
				O_CREAT | O_EXCL, 
				S_IRWXU | S_IRGRP | S_IROTH, 
				INTVAL ))) {
			printf( "%s\n", strerror( errno ));
			exit( ERR );
		}
		
		// create a shared memory segment here
		if( ERR == ( shmfd = shm_open( 
				"/tmpshm", 
				O_RDWR | O_CREAT,
				S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH
			))) {
			printf( "%s\n", strerror( errno ));exit( ERR );
		}

		// the newly created shared memory segment is 0, so
		// resize it using ftruncate
		if( ERR == ftruncate( shmfd, SHMBUFFLEN )) {
			printf( "%s\n", strerror( errno ));
			exit( ERR );
		}
		
		// get the address of the shared memory segment 
		if( MAP_FAILED == ( shmBuff = ( char * )  mmap ( 
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


		// in an endless loop...
		for( ;; ) {
			// prompt the user for input here...
			fgets( inputBuff, INPUTBUFFSZ - 1, stdin );
			
			// acquire the semaphore
			sem_wait( sem );
			
			// place the input from the user into the shared memory segment
			strcpy( shmBuff + strlen( shmBuff ), inputBuff );
			strcpy( shmBuff + strlen( shmBuff ), "\n" );

			// release the semaphore
			sem_post( sem );
		}
	}

	exit( 0 );
}

