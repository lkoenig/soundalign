#include "circular_buffer.h"

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
    assert(novelty->size() <= buffer_->size());
    size_t last = max(buffer_->size() - novelty.size(), 0);
    
    // Move the end of the buffer to the beginning
    
    // Append novelty
    buffer_->insert(buffer_[last], novelty);
}

template<class T>
T * CircularBuffer<T>::data()
{
    return buffer_->data();
}
