#include <ParserConfig.hpp>

void ParserConfig::readConfigFile(const char* m_configFile) {
    this->m_configfile = m_configFile;
    std::ifstream cFile (this->m_configfile);
    if (cFile.is_open())
    {
        std::string line;
        while(getline(cFile, line)) {

            auto delimiterPos = line.find("=");
            if (delimiterPos == std::string::npos) {
                continue;
            }

            auto name = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + 1);

            if (name == "window_title") {
                m_cVars[name] = value; // Garder les espaces
            } else {
                // Enlèvement des "espaces blancs" (ex: ab=cd''' / ab''=''cd  == ab=cd)
                line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
                if(line[0] == '#' || line.empty()) {
                    continue;
                }

                // Equivalence (ex: ab=cd  ==  ab=cd)
                auto delimiterPos = line.find("=");
                auto name = line.substr(0, delimiterPos);
                auto value = line.substr(delimiterPos + 1);

                try {
                    if (value.find('.') != std::string::npos) {
                        if (value.length() - value.find('.') > 2) {
                            m_cVars[name] = std::stod(value); // Double
                        } else {
                            m_cVars[name] = std::stof(value); // Float
                        }
                    } else {
                        m_cVars[name] = std::stoi(value); // Int
                    }
                } catch (std::invalid_argument&) {
                    m_cVars[name] = value; // String
                }
                            
            }
        }
    } 
}