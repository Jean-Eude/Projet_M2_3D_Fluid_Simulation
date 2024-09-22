#include <Allocator.hpp>

Allocator::Allocator(const std::size_t totalSize) : m_totalSize(totalSize), m_used(0), m_peak(0) {

}