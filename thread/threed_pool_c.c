

typedef struct Task {
    void (*function)(void *arg);
    void *arg;
} Task;

struct ThreadPool {
    Task *taskQ;  //ask queue
    int queueCapacity;
    int queueSize;
    int QueueFront;
    int QueueRear;

    pthread_t managerID;
    pthread_t *threadIDs;
    int minNum;
    int maxNum;
    int busyNum;
    int liveNum;
    int exitNum;
    pthread_mutex_t mutexPool;
    pthread_mutex_t mutexBusy;
    pthread_cond_t notFull;
    pthread_cond_t not empty;

    int shutdown;
};
