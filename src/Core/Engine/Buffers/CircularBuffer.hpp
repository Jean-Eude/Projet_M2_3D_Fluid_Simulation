#pragma once

#include <HeadersBase.hpp>
#include <Node.hpp>

template <class T>
class CircularBuffer {
private:
    Node<T>* head;
    Node<T>* tail;
    size_t size;
    size_t capacity;

public:
    CircularBuffer(size_t cap);
    ~CircularBuffer();
    void enqueue(T data);
    T dequeue();

    bool isEmpty() const;
    bool isFull() const;

    size_t getSize() const;
    size_t getCapacity() const;
};

template <class T>
CircularBuffer<T>::CircularBuffer(size_t cap) : head(nullptr), tail(nullptr), size(0), capacity(cap) {}

template <class T>
CircularBuffer<T>::~CircularBuffer() {
    while (!isEmpty()) {
        dequeue();
    }
}

template <class T>
void CircularBuffer<T>::enqueue(T data) {
    Node<T>* newNode = new Node<T>(data);

    if (isEmpty()) {
        head = newNode;
        tail = newNode;
        tail->next = head;
    } else {
        tail->next = newNode;
        tail = newNode;
        tail->next = head;  
    }

    if (isFull()) {
        Node<T>* temp = head;
        head = head->next;
        tail->next = head;
        delete temp;
    } else {
        ++size;
    }
}

template <class T>
T CircularBuffer<T>::dequeue() {
    if (isEmpty()) {
        throw std::runtime_error("Buffer is empty");
    }

    T data = head->data;
    Node<T>* temp = head;
    if (size == 1) {
        head = nullptr;
        tail = nullptr;
    } else {
        head = head->next;
        tail->next = head;
    }
    delete temp;
    --size;

    return data;
}

template <class T>
bool CircularBuffer<T>::isEmpty() const {
    return size == 0;
}

template <class T>
bool CircularBuffer<T>::isFull() const {
    return size == capacity;
}

template <class T>
size_t CircularBuffer<T>::getSize() const {
    return size;
}

template <class T>
size_t CircularBuffer<T>::getCapacity() const {
    return capacity;
}