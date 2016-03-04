#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <cstddef>
#include <vector>
#include <memory>

template<class T>
class CircularBuffer {
    std::vector<T> buffer_;

public:
    CircularBuffer(size_t size);
    ~CircularBuffer() {};

    void append(std::vector<T> novelty);
    T* data();
};


template<class T>
CircularBuffer<T>::CircularBuffer(size_t size)
{
    this->buffer_ = std::vector<T>(size);
    for (size_t i = 0; i < size; i++) {
        this->buffer_[i] = 0;
    }
}

template<class T>
void CircularBuffer<T>::append(std::vector<T> novelty)
{
    assert(novelty.size() <= buffer_.size());
    size_t last = buffer_.size() - novelty.size();
    
    // Move the end of the buffer to the beginning
    memmove(buffer_.data(), &buffer_[novelty.size()], last * sizeof(T));
    
    // Append novelty
    memcpy(&buffer_[last], novelty.data(), sizeof(T) * novelty.size()); 
}

template<class T>
T * CircularBuffer<T>::data()
{
    return buffer_.data();
} 



#endif
