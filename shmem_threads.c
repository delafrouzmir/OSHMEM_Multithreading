#include "shmem_threads.h"


void shmem_thread_init( void(*f)(void*), void *args, int numThreads, int peNum )
{
	int nthreads, tid;

	#pragma omp parallel private(nthreads, tid) num_threads(numThreads+1)
	{
		tid = omp_get_thread_num();
		nthreads = omp_get_num_threads();
		
		if ( tid == nthreads -1 )
		{
			printf("polling for exit in PE %d\n", peNum);
			// gonna wait for multi-threading finish alert here
		}
		else
		{
			//printf("from thread %d :\n", tid );
			f(args, tid);
		}
	}
	
}