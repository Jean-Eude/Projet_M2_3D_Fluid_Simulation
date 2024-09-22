#pragma once

#include <Allocator.hpp>

// FIFO (ex : CustomQueue)

class LinearAllocator : public Allocator {
    public:
        LinearAllocator(const std::size_t totalSize);
        LinearAllocator(LinearAllocator &linearAllocator);        
        virtual ~LinearAllocator();
        virtual void* Allocate(const std::size_t size, const std::size_t alignment = 0) override;
        virtual void Free(void* ptr) override;
        virtual void Init() override;
	    virtual void Reset();

    private:
        void* m_ptr_start;
        std::size_t m_offset;
};