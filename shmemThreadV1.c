#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <omp.h>
#include <sys/utsname.h>
#include <shmem.h>


void helloWorld (int x, int thNum, int peNum, char *nodeName)
{
	int i;
	for (i=0; i<x; ++i)
	{
		printf("%s: Hello World for the %d_th time from thread %d of PE %d \n", nodeName, i, thNum, peNum);
	}
}

void shmem_thread_init( void(*f)(int,int,int,char *), int x, int numThreads, int peNum, char *nodeName )
{
	int nthreads, tid;

	#pragma omp parallel private(nthreads, tid) num_threads(numThreads+1)
	{
		tid = omp_get_thread_num();
		nthreads = omp_get_num_threads();
		
		if ( tid == nthreads -1 )
		{
			printf("polling for exit in PE %d node %s \n", peNum, nodeName);
			// gonna wait for multi-threading finish alert here
		}
		else
		{
			//printf("from thread %d :\n", tid );
			f(3, tid, peNum, nodeName);
		}
	}
	
}

int main(int argc, char const *argv[])
{
	int me, npes, nthreads, tid;
    struct utsname u;

    uname (&u);

    shmem_init ();

    me = shmem_my_pe ();
    npes = shmem_n_pes ();

	void (*funcPtr)(int,int,int,char*) = &helloWorld;
	shmem_thread_init(funcPtr, 3, 4, me, u.nodename);

	shmem_finalize ();

	return 0;
}
