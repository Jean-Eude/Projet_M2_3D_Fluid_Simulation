#pragma once

#include <HeadersBase.hpp>

class Alignement {
    public:
        static const std::size_t CalculatePadding(const std::size_t baseAddress, const std::size_t alignment);
        static const std::size_t CalculatePaddingWithHeader(const std::size_t baseAddress, const std::size_t alignment, const std::size_t headerSize);
};