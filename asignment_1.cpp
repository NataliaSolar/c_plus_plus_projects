/**
 * @author Natalia Solar
 * CSD415 Section: H21
 * Homework Assignment 1
 * A program that queries at least 8 process limits on your system, and prints the 
 * soft and hard values for each resource you chose.
 */

/* This demonstrates how to call getrlimit()
 * getrlimit() takes two parameters:
 *      a resource constant
 *      the address of an rlimit struct
 *
 * Here are some resource constants:
 *      RLIMIT_AS       max size of process total available memory
 *      RLIMIT_DATA     max size of data segment
 *      RLIMIT_NPROC    max number of child processes per real user ID
 *      RLIMIT_STACK    max size of the stack
 *
 * The rlimit struct looks like this:
 *      struct limit {
 *              rlim_t rlim_cur;    // soft limit: current limit
 *              rlim_t rlim_max;    // hard limit: max val for rlim_cur }
 *
 * As with most Linux calls, for more information use
 *      $ man getrlimit
 */


#include <sys/resource.h>
#include <iostream>

using namespace std;

int main(void) {
	struct rlimit limit;

	/**
	 * RLIMIT_AS
     * This is the maximum size of the process's virtual memory (address space).  The limit  is  specified  in
     * bytes,  and  is rounded down to the system page size.  This limit affects calls to brk(2), mmap(2), and
     * mremap(2), which fail with the error ENOMEM upon exceeding this limit.  In  addition,  automatic  stack
     * expansion  fails  (and  generates  a SIGSEGV that kills the process if no alternate stack has been made
     * available via sigaltstack(2)).
	*/
	if (getrlimit(RLIMIT_AS, &limit) < 0) {
		cout << "...oops..." << endl;
	}
	else {
		cout << "RLIMIT_AS" << endl;
		cout << "\tsoft limit:" << limit.rlim_cur << endl;
		cout << "\thard limit:" << limit.rlim_max << endl;
	}


	/**
	 * RLIMIT_CORE
     * This  is  the  maximum size of a core file (see core(5)) in bytes that the process may dump.  When 0 no
     * core dump files are created.  When nonzero, larger dumps are truncated to this size.
	*/
	if (getrlimit(RLIMIT_CORE, &limit) < 0) {
		cout << "...oops..." << endl;
	}
	else {	
		cout << "RLIMIT_CORE" << endl;
		cout << "\tsoft limit:" << limit.rlim_cur << endl;
		cout << "\thard limit:" << limit.rlim_max << endl;
	}


	/**
	 * RLIMIT_CPU
     * This is a limit, in seconds, on the amount of CPU time that the process can consume.  When the  process
     * reaches  the  soft limit, it is sent a SIGXCPU signal.  The default action for this signal is to termi-
     * nate the process.  However, the signal can be caught, and the handler can return control  to  the  main
     * program.   If  the process continues to consume CPU time, it will be sent SIGXCPU once per second until
     * the hard limit is reached, at which time it is sent SIGKILL.
	*/
	if (getrlimit(RLIMIT_CPU, &limit) < 0) {
		cout << "...oops..." << endl;
	}
	else {
		cout << "RLIMIT_CPU" << endl;
		cout << "\tsoft limit:" << limit.rlim_cur << endl;
		cout << "\thard limit:" << limit.rlim_max << endl;
	}


	/*
	 * RLIMIT_DATA
     * This  is  the  maximum  size  of  the process's data segment (initialized data, uninitialized data, and
     * heap).  The limit is specified in bytes, and is rounded down to  the  system  page  size. This  limit
     * affects  calls to brk(2), sbrk(2), and mmap(2), which fail with the error ENOMEM upon
	 * encountering the soft limit of this resource.
	*/
	if (getrlimit(RLIMIT_DATA, &limit) < 0) {
		cout << "...oops..." << endl;
	}
	else {
		cout << "RLIMIT_DATA" << endl;
		cout << "\tsoft limit:" << limit.rlim_cur << endl;
		cout << "\thard limit:" << limit.rlim_max << endl;
	}


	/**
	 * RLIMIT_FSIZE
     * This is the maximum size in bytes of files that the process may create.   Attempts  to  extend  a  file
     * beyond  this  limit  result  in  delivery  of  a  SIGXFSZ signal.  By default, this signal terminates a
     * process, but a process can catch this signal instead, in which case the  relevant  system  call.
	 */
	if (getrlimit(RLIMIT_FSIZE, &limit) < 0) {
		cout << "...oops..." << endl;
	}
	else {
		cout << "RLIMIT_FSIZE" << endl;
		cout << "\tsoft limit:" << limit.rlim_cur << endl;
		cout << "\thard limit:" << limit.rlim_max << endl;
	}


	/**
	 * RLIMIT_LOCKS
     * This  is  a  limit  on  the combined number of flock(2) locks and fcntl(2) leases that 
	 * this process may establish.
	*/
	if (getrlimit(RLIMIT_LOCKS, &limit) < 0) {
		cout << "...oops..." << endl;
	}
	else {
		cout << "RLIMIT_LOCKS" << endl;
		cout << "\tsoft limit:" << limit.rlim_cur << endl;
		cout << "\thard limit:" << limit.rlim_max << endl;
	}


	/**
	 * RLIMIT_MEMLOCK
     * This is the maximum number of bytes of memory that may be locked into RAM.  This  limit  is  in  effect
	 * rounded  down  to  the  nearest  multiple of the system page size.
	*/
	if (getrlimit(RLIMIT_MEMLOCK, &limit) < 0) {
		cout << "...oops..." << endl;
	}
	else {
		cout << "RLIMIT_MEMLOCK" << endl;
		cout << "\tsoft limit:" << limit.rlim_cur << endl;
		cout << "\thard limit:" << limit.rlim_max << endl;
	}


	/**
	 * RLIMIT_MSGQUEUE
	 * This is a limit on the number of bytes that can be allocated for POSIX message queues for the real user
	 * ID of the calling process.  This limit is enforced for mq_open(3).  Each message queue  that  the  user
	 * creates counts (until it is removed) against this limit.
	*/
	if (getrlimit(RLIMIT_MSGQUEUE, &limit) < 0) {
		cout << "...oops..." << endl;
	}
	else {
		cout << "RLIMIT_MSGQUEUE" << endl;
		cout << "\tsoft limit:" << limit.rlim_cur << endl;
		cout << "\thard limit:" << limit.rlim_max << endl;
	}


	/**
	 * RLIMIT_NICE
	 * This specifies a ceiling to which the process's nice  value  can  be  raised  using  setpriority(2)  or
	 * nice(2).   The  actual ceiling for the nice value is calculated as 20 - rlim_cur.  The useful range for
	 * this limit is thus from 1 (corresponding to a nice value of 19) to 40 (corresponding to a nice value of -20).  
	*/
	if (getrlimit(RLIMIT_NICE, &limit) < 0) {
		cout << "...oops..." << endl;
	}
	else {
		cout << "RLIMIT_NICE" << endl;
		cout << "\tsoft limit:" << limit.rlim_cur << endl;
		cout << "\thard limit:" << limit.rlim_max << endl;
	}


	/**
	* RLIMIT_RSS:
	* This is a limit (in bytes) on the process's resident set (the number of virtual pages resident in RAM).
	* This limit has effect only in Linux 2.4.x, x < 30, and there affects only calls to 
	* madvise(2)  specifying MADV_WILLNEED.
	*/
	if (getrlimit(RLIMIT_AS, &limit) < 0) {
		cout << "...oops..." << endl;
	}
	else {
		cout << "RLIMIT_AS" << endl;
		cout << "\tsoft limit:" << limit.rlim_cur << endl;
		cout << "\thard limit:" << limit.rlim_max << endl;
	}
}