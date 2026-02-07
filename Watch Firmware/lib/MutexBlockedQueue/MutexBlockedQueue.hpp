#ifndef MUTEX_BLOCKED_QUEUE_H
#define MUTEX_BLOCKED_QUEUE_H

#include <Thread.h>

class MutexBlockedQueue
{
private:
    Thread *selectedThread = nullptr;
    int *items = nullptr;
    int first = 0;
    int last = 0;
    int size = 0;

public:
    const int SUCCESS = 0;
    const int FAILURE = -1;
    const int BLOCKED = 1;

    /*
    Creates instance of `MutexBlockedQueue`.
    `size` indicates the max number of entries
    */
    MutexBlockedQueue(int queue_size);

    ~MutexBlockedQueue();

    /*
    Pushes value onto queue.

    Returns `SUCCESS` if successful.
    Returns `FAILURE` if failure.
    Returns `BLOCKED` if blocked.
    */
    int push(int value, Thread *currentThread);

    /*
    Pops value from queue.

    Returns `SUCCESS` if successful.
    Returns `FAILURE` if failure.
    Returns `BLOCKED` if blocked.
    */
    int pull(int &value, Thread *currentThread);
};

#endif