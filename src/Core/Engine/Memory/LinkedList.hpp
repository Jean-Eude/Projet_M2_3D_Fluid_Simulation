#pragma once

#include <HeadersBase.hpp>

template <class T>
class LinkedList {
    public:
        struct Node {
            T data;
            Node* next;
        };
        
        Node* head;

        LinkedList() = default;
        LinkedList(LinkedList &LinkedList) = delete;
        void push(Node * newNode);
        Node* pop();
};

template <class T>
void LinkedList<T>::push(Node * newNode) {
    newNode->next = head;
    head = newNode;
}

template <class T>
typename LinkedList<T>::Node* LinkedList<T>::pop() {
    Node * top = head;
    head = head->next;
    return top;
}