#pragma once

#include <HeadersBase.hpp>

template <class T>
struct Node {
    T data;
    Node* next;
    
    Node(T d) : data(d), next(nullptr) {}
};