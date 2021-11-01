#include <iostream>
#include <csignal>
#include <unistd.h>

using namespace std;


static void sig_usr( int );	// signal handler

int main( void ) {
	pid_t pid = getpid();

	if( signal( SIGUSR1, sig_usr ) == SIG_ERR ) {
		cerr << "can't catch SIGUSR1" << endl;
	}
	if( signal( SIGUSR2, sig_usr ) == SIG_ERR ) {
		cerr << "can't catch SIGUSR2" << endl;
	}

	cout << "pid=" << pid << endl;

	for( ; ; ) {
		sleep( 1 );
		cout << "...tick..." << endl;
	}
}

static void sig_usr( int signo ) {
	if( signo == SIGUSR1 ) {
		cout << "received SIGUSR1" << endl;
	} else if( signo == SIGUSR2 ) {
		cout << "received SIGUSR2" << endl;
	} else {
		cerr << "...oops..." << endl;
	}
}

