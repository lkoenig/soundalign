#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H
#include <vector>

template<class T>
class CircularBuffer {
    std::vector<T> buffer_;

public:
    CircularBuffer(size_t size);
    ~CircularBuffer() {};

    void append(std::vector<T> novelty);
    T* data();
};

#endif