#include <FilePath.hpp>

std::string FilePath::getFilePath(const std::string& filepath) {
    std::string basePath = std::filesystem::current_path().parent_path().string();
    std::cout << basePath + filepath << std::endl;
    return basePath + filepath;
}