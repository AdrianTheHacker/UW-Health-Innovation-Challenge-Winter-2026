#include "SafeQueue.h"

SafeQueue::SafeQueue(int size) : _size(size), _head(0), _tail(0), _count(0), _isLocked(false) {
    _buffer = new int[size];
}

SafeQueue::~SafeQueue() {
    delete[] _buffer;
}

void SafeQueue::lock() {
    // Basic atomic-style lock for Arduino
    while (_isLocked) { yield(); } 
    _isLocked = true;
}

void SafeQueue::unlock() {
    _isLocked = false;
}

bool SafeQueue::push(int value) {
    lock();
    if (_count == _size) {
        unlock();
        return false; // Queue Full
    }
    
    _buffer[_tail] = value;
    _tail = (_tail + 1) % _size; // Wrap around
    _count++;
    
    unlock();
    return true;
}

bool SafeQueue::pop(int &value) {
    lock();
    if (_count == 0) {
        unlock();
        return false; // Queue Empty
    }
    
    value = _buffer[_head];
    _head = (_head + 1) % _size; // Wrap around
    _count--;
    
    unlock();
    return true;
}

bool SafeQueue::isEmpty() {
    return _count == 0;
}