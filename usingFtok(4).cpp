#include <sys/ipc.h>
#include <iostream>

using namespace std;

int main() {
	key_t ipcKey;

	ipcKey = ftok( "/home/tomabot/local/ipc/usingFtok/ipc.key", 'a' );
	cout << "ipcKey=" << ipcKey << endl;

	return( 0 );
}
