#pragma once

#include <Allocator.hpp>
#include <LinkedList.hpp>

class PoolAllocator : public Allocator {
    private:
        struct  FreeHeader{
        };
        
        using Node = LinkedList<FreeHeader>::Node;
        LinkedList<FreeHeader> m_freeList;

        void * m_ptr_start;
        std::size_t m_chunkSize;
        
    public:
        PoolAllocator(const std::size_t totalSize, const std::size_t chunkSize);
        PoolAllocator(PoolAllocator &poolAllocator);

        virtual ~PoolAllocator();
        virtual void* Allocate(const std::size_t size, const std::size_t alignment = 0) override;
        virtual void Free(void* ptr) override;
        virtual void Init() override;
        virtual void Reset();
};