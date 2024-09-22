#pragma once

#include <HeadersBase.hpp>

// Classe qui permet de lire un fichier de configuration et de stocker les variables / valeurs dans un map

class ParserConfig {
    public:
        using CommandType = std::variant<int, double, float, std::string>;

        virtual ~ParserConfig() = default;

        // Fonction pour read
        void readConfigFile(const char* configfile);

        // Variables
        std::string m_configfile;
        std::map<std::string, CommandType> m_cVars;
};