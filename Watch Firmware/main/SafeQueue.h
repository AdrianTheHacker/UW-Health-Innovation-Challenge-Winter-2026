#ifndef SAFE_QUEUE_H
#define SAFE_QUEUE_H

#include <Arduino.h>

// If using Arduino_Threads/RTOS, we use the built-in Mutex
// If using a simpler scheduler, we use a critical section lock
class SafeQueue {
public:
    SafeQueue(int size);
    ~SafeQueue();

    bool push(int value);   // Add to queue
    bool pop(int &value);    // Remove from queue
    bool isEmpty();

private:
    int* _buffer;
    int _size;
    int _head;
    int _tail;
    int _count;
    
    // Mutex to ensure thread safety
    volatile bool _isLocked; 
    void lock();
    void unlock();
};

#endif