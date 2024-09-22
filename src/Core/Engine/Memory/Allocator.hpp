#pragma once

#include <HeadersBase.hpp>

class Allocator {
    public:
        std::size_t m_totalSize;
        std::size_t m_used;   
        std::size_t m_peak;
        
        Allocator(const std::size_t totalSize);
        virtual ~Allocator() { m_totalSize = 0; }
        virtual void* Allocate(const std::size_t size, const std::size_t alignment = 0) = 0;
        virtual void Free(void* ptr) = 0;
        virtual void Init() = 0;
};