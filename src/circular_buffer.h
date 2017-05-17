#pragma once
#include <cstddef>
#include <vector>
#include <memory>

template<class T>
class CircularBuffer {
    std::vector<T> buffer_;

public:
    CircularBuffer(size_t size);
    ~CircularBuffer() {};

    void append(const std::vector<T> &novelty);
    void append(const T* novelty, size_t count);

    size_t size() {return buffer_.size(); }
    size_t sizeInBytes() {return buffer_.size() * sizeof(T); }

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
void CircularBuffer<T>::append(const std::vector<T> &novelty)
{
    CircularBuffer<T>::append(novelty.data(), novelty.size());
}

template<class T>
void CircularBuffer<T>::append(const T* novelty, size_t count)
{
    assert(count <= buffer_.size());
    size_t last = buffer_.size() - count;
    
    // Move the end of the buffer to the beginning
    memmove(buffer_.data(), &buffer_[count], last * sizeof(T));
    
    // Append novelty
    memcpy(&buffer_[last], novelty, sizeof(T) * count);
}

template<class T>
T * CircularBuffer<T>::data()
{
    return buffer_.data();
} 

