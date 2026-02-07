#include "MutexBlockedQueue.hpp"

MutexBlockedQueue::MutexBlockedQueue(int queue_size) {
    items = new int[queue_size + 1]();
    first = 0;
    last = 1;
    size = queue_size + 1;
}

MutexBlockedQueue::~MutexBlockedQueue() {
    delete[] items;
    items = nullptr;

    selectedThread = nullptr;
}

int MutexBlockedQueue::push(int value, Thread *currentThread) {
    if(selectedThread != nullptr && selectedThread != currentThread) {
        // Queue is blocked.
        return BLOCKED;
    }

    selectedThread = currentThread;
    if(first == last + 1 || (last == size - 1 && first == 0)) {
        // Queue is full.
        return FAILURE;
    }

    last -= 1;
    if(last < 0) {
        last = size -1;
    }
    items[last] = value;
    last += 1;
    
    if(last == size) {
        last = 0;
    }

    selectedThread = nullptr;
    return SUCCESS;
}

int MutexBlockedQueue::pull(int &value, Thread *currentThread) {
    if(selectedThread != nullptr && selectedThread != currentThread) {
        return BLOCKED;
    }

    selectedThread = currentThread;

    if(last == first + 1) {
        // Queue is empty.
        return FAILURE;
    }

    value = items[first];
    first += 1;
    if(first == size) {
        first = 0;
    }

    selectedThread = nullptr;
    return SUCCESS;
}

