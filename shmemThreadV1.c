#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <omp.h>
#include <sys/utsname.h>
#include <shmem.h>

#include "shmem_threads.h"


typedef struct {
    int iter;
    int peNum;
    char *nodeName;
} myfunc_params;

void helloWorld (void *args, /*int x, int peNum, char *nodeName,*/ int thNum)
{
	//getting the parameters to this function
	myfunc_params *params = (myfunc_params *)args;
	int x = params->iter;
	int peNum = params->peNum;
	char *nodeName = params->nodeName;
	
	int i;
	for (i=0; i<x; ++i)
	{
		printf("%s: Hello World for the %d_th time from thread %d of PE %d \n", nodeName, i, thNum, peNum);
	}
}


int main(int argc, char const *argv[])
{
	int me, npes, nthreads, tid;
    struct utsname u;

    uname (&u);

    // starting OpenSHMEM
    shmem_init ();

    // getting the number of PEs and the current PE's number
    npes = shmem_n_pes ();
    me = shmem_my_pe ();
    
    // creating a pointer to the function helloWorld, to pass to shmem_thread_init
	void (*funcPtr)(void*) = &helloWorld;
	// creating a struct of helloWorld's arguments
	myfunc_params f_args = {3, me, u.nodename};

	// starting multi-threading mode with 5+1 threads, where 4 threads call funcPtr(f_args)
	shmem_thread_init(funcPtr, &f_args, 4);

	// finalizing OpenSHMEM
	shmem_finalize ();

	return 0;
}
