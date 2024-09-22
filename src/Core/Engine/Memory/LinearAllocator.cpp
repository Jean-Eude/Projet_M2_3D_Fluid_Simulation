#include <LinearAllocator.hpp>
#include <Alignement.hpp>

LinearAllocator::LinearAllocator(const std::size_t totalSize) : Allocator(totalSize), m_ptr_start(nullptr), m_offset(0) {

}

void LinearAllocator::Init() {
    if (m_ptr_start != nullptr) {
        free(m_ptr_start);
    }
    m_ptr_start = malloc(m_totalSize);
    if (m_ptr_start == nullptr) {
        throw std::bad_alloc();
    }
    m_offset = 0;
}

LinearAllocator::~LinearAllocator() {
    if (m_ptr_start) {
        free(m_ptr_start);
        m_ptr_start = nullptr;  // Mettez le pointeur à nullptr après la libération pour éviter les doubles libérations.
    }
}

void* LinearAllocator::Allocate(const std::size_t size, const std::size_t alignment) {
    std::size_t padding = 0;
    std::size_t paddedAddress = 0;
    const std::size_t currentAddress = reinterpret_cast<std::size_t>(m_ptr_start) + m_offset;

    if (alignment != 0 && currentAddress % alignment != 0) {
        padding = Alignement::CalculatePadding(currentAddress, alignment);
    }

    if (m_offset + padding + size > m_totalSize) {
        return nullptr;  // Évitez de dépasser la taille totale
    }

    m_offset += padding;
    const std::size_t nextAddress = currentAddress + padding;
    m_offset += size;

    #ifdef _DEBUG
        std::cout << "A" << "\t@C " << (void*) currentAddress << "\t@R " << (void*) nextAddress << "\tO " << m_offset << "\tP " << padding << std::endl;
    #endif

    m_used = m_offset;
    m_peak = std::max(m_peak, m_used);

    return (void*) nextAddress;
}

void LinearAllocator::Free(void* ptr) {
    assert(false && "Use Reset() method");
}

void LinearAllocator::Reset() {
    m_offset = 0;
    m_used = 0;
    m_peak = 0;
}