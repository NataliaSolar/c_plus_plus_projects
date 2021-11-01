#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>

using namespace std;

#define INTVAL 1
#define ERR -1
#define SHMBUFFLEN 256

sem_t *sem;
int shmfd;
pid_t pid;

char *shm_buffer;

char inStr[ SHMBUFFLEN - 1 ];

int main() {

	// attach to a named semaphore
	if( SEM_FAILED == ( sem = sem_open( 
			"/tmpsem", 
			O_CREAT, 
			S_IRWXU | S_IRGRP | S_IROTH, 
			INTVAL 
		))) {
		printf( "sem_open: %s\n", strerror( errno ));
		exit( ERR );
	}

        // create a POSIX shared memory segment
        if( ERR == ( shmfd = shm_open(
                        "/tmpshm",
                        O_RDWR ,
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

	printf( "enter some text to send to writer: " );
	while( NULL != fgets( inStr, 64, stdin ))  {
		// issue a sem_wait on the semaphore and then add 
		// message to it
		if( ERR == sem_wait( sem )) {
			printf( " sem_wait returned error\n" );
			exit( ERR );
		}

		printf( "...putting message in the shared memory segment...\n" );
		for( int i = 0; i < ( int ) strlen( inStr ); i++ ) {
			*( shm_buffer + i )  = *( inStr + i );
		}
		*( shm_buffer + strlen( inStr ) - 1 ) = '\0';

		pid = getpid();
	
		if( ERR == sem_post( sem )) {
			printf( " sem_post returned error\n" );
			exit( ERR );
		}
		printf( "enter some text to send to writer: " );
	} 

	printf( "...done...\n" );
}


