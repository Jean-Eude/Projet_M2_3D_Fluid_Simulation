#pragma once

// Classe qui permet de raccourcir les paths de fichiers

#include <HeadersBase.hpp>

class FilePath {
    public :
        FilePath() = delete;
        ~FilePath() = default;

        static std::string getFilePath(const std::string& filepath);
};