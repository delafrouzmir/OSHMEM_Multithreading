#ifndef __SHMEM_THREADS_H_INCLUDED__
#define __SHMEM_THREADS_H_INCLUDED__

void shmem_thread_init( void(*f)(void*), void *args, int numThreads, int peNum );
void shmem_thread_finalize(void);

#endif