#ifndef _THREADPOOL_H
typedef struct ThreadPool ThreadPool;
ThreadPool *threadPoolCreate(int min, int max, int queueSize);
int threadPoolDestroy(ThreadPool *pool);
void threadPoolAdd(ThreadPool *ppol, void (*func)(void*), void *arg);
int threadPoolBusyNum(ThreadPool *pool);
int threadPoolAliveNum(ThreadPool *pool);

//consumer
void *worker(void *arg);
//manager
void *manager(void *arg);
void threadExit(ThreadPool *pool);

ThreadPool *threadPoolCreate(int min, int max, int queueSize) {
    ThreadPool *pool =  (ThreadPool*)malloc(sizeof(ThreadPool));
}



#endif // _THREADPOOL_H
