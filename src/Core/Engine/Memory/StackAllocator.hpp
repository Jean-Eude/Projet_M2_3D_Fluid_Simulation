#pragma once

#include <Allocator.hpp>

// LIFO

class StackAllocator : public Allocator {
    protected:
        void* m_ptr_start;
        std::size_t m_offset;
    public:
        StackAllocator(const std::size_t totalSize);
        StackAllocator(StackAllocator &stackAllocator);

        virtual ~StackAllocator();
        virtual void* Allocate(const std::size_t size, const std::size_t alignment = 0) override;
        virtual void Free(void* ptr);
        virtual void Init() override;
        virtual void Reset();

    private:
        struct AllocationHeader {
            char padding;
        };

};