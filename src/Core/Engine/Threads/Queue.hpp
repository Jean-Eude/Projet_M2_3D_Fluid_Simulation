#pragma once

#include <LinearAllocator.hpp>
#include <Node.hpp>

template <class T>
class Queue {
    public:
        Queue() = default;
        Queue(LinearAllocator* allocator);

        void push(T const& data);
        T pop();
        bool isEmpty() const;
        void reset();
        T& fronte() const;
        
        template <class... Args>
        void emplace(Args&&... args);

    private:
        Node<T>* front;
        Node<T>* rear;
        LinearAllocator* allocator;
        mutable std::mutex _queue_mutex;
        std::condition_variable _queue_cv;
};


template <class T>
Queue<T>::Queue(LinearAllocator* allocator) : front(nullptr), rear(nullptr), allocator(allocator) {}

template <class T>
void Queue<T>::push(T const& data) {
    if (!allocator) {
        throw std::runtime_error("Allocator not set");
    }
    Node<T>* newNode = static_cast<Node<T>*>(allocator->Allocate(sizeof(Node<T>), alignof(Node<T>)));
    if (!newNode) {
        std::cout << "Queue Overflow!" << std::endl;
        return;
    }
    newNode->data = data;
    newNode->next = nullptr;

    {
        std::lock_guard<std::mutex> lock(_queue_mutex);
        if (rear == nullptr) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
    }

    _queue_cv.notify_one();
}

template <class T>
T Queue<T>::pop() {
    std::unique_lock<std::mutex> lock(_queue_mutex);
    _queue_cv.wait(lock, [this] { return !isEmpty(); });

    Node<T>* temp = front;
    front = front->next;
    if (front == nullptr) {
        rear = nullptr;
    }

    T data = temp->data;
    temp->~Node<T>();  // Appeler explicitement le destructeur
    // Ne pas désallouer manuellement la mémoire avec LinearAllocator

    return data;
}

template <class T>
bool Queue<T>::isEmpty() const {
    if(front == nullptr) {
        return true;
    } else {
        return false;
    }
}

template <class T>
void Queue<T>::reset() {
    std::lock_guard<std::mutex> lock(_queue_mutex);

    while (!isEmpty()) {
        Node<T>* temp = front;
        front = front->next;
        temp->~Node<T>(); // Détruire chaque nœud
        // Note: La mémoire n'est pas explicitement libérée avec LinearAllocator
    }
    rear = nullptr;
    allocator->Reset();
}

template <class T>
T& Queue<T>::fronte() const {
    std::lock_guard<std::mutex> lock(_queue_mutex);

    if (isEmpty()) {
        throw std::runtime_error("Queue is empty!");
    }

    return front->data;
}

template <class T>
template <class... Args>
void Queue<T>::emplace(Args&&... args) {
    Node<T>* newNode = static_cast<Node<T>*>(allocator->Allocate(sizeof(Node<T>), alignof(Node<T>)));
    if (!newNode) {
        std::cerr << "Queue Overflow! Allocation failed." << std::endl;
        return;
    }

    try {
        new(&newNode->data) T(std::forward<Args>(args)...);
    } catch (...) {
        // Si une exception se produit, on ne peut pas désallouer la mémoire avec le LinearAllocator
        std::cerr << "Failed to construct object in-place." << std::endl;
        return;
    }

    newNode->next = nullptr;

    {
        std::lock_guard<std::mutex> lock(_queue_mutex);
        if (rear == nullptr) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
    }

    _queue_cv.notify_one();
}