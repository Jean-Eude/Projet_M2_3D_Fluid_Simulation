#include <StackAllocator.hpp>
#include <Alignement.hpp>

StackAllocator::StackAllocator(const std::size_t totalSize) : Allocator(totalSize), m_ptr_start(nullptr), m_offset(0) {

}

void StackAllocator::Init() {
    if (m_ptr_start != nullptr) {
        free(m_ptr_start);
    }
    m_ptr_start = malloc(m_totalSize);
    m_offset = 0;
}

StackAllocator::~StackAllocator() {
    free(m_ptr_start);
    m_ptr_start = nullptr;
}

void* StackAllocator::Allocate(const std::size_t size, const std::size_t alignment) {
    const std::size_t currentAddress = (std::size_t)m_ptr_start + m_offset;

    std::size_t padding = Alignement::CalculatePaddingWithHeader(currentAddress, alignment, sizeof (AllocationHeader));

    if (m_offset + padding + size > m_totalSize) {
        return nullptr;
    }
    m_offset += padding;

    const std::size_t nextAddress = currentAddress + padding;
    const std::size_t headerAddress = nextAddress - sizeof (AllocationHeader);
    AllocationHeader allocationHeader{padding};
    AllocationHeader* headerPtr = (AllocationHeader*) headerAddress;
    headerPtr = &allocationHeader;
    
    m_offset += size;

    #ifdef _DEBUG
        std::cout << "A" << "\t@C " << (void*) currentAddress << "\t@R " << (void*) nextAddress << "\tO " << m_offset << "\tP " << padding << std::endl;
    #endif
    m_used = m_offset;
    m_peak = std::max(m_peak, m_used);

    return (void*) nextAddress;
}

void StackAllocator::Free(void *ptr) {
    const std::size_t currentAddress = (std::size_t) ptr;
    const std::size_t headerAddress = currentAddress - sizeof (AllocationHeader);
    const AllocationHeader * allocationHeader{ (AllocationHeader *) headerAddress};

    m_offset = currentAddress - allocationHeader->padding - (std::size_t) m_ptr_start;
    m_used = m_offset;

    #ifdef _DEBUG
        std::cout << "F" << "\t@C " << (void*) currentAddress << "\t@F " << (void*) ((char*) m_ptr_start + m_offset) << "\tO " << m_offset << std::endl;
    #endif
}

void StackAllocator::Reset() {
    m_offset = 0;
    m_used = 0;
    m_peak = 0;
}