#include <FilePath.hpp>

std::string FilePath::getFilePath(const std::string& filepath) {
    std::string basePath = std::filesystem::current_path().parent_path().string();
    return basePath + filepath;
}