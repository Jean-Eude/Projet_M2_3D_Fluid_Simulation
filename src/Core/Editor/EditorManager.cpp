#include <EditorManager.hpp>

void EditorManager::OnReadConfigFile(const char* cfg) {
    readConfigFile(cfg);
    
    for (const auto& a : m_cVars) {
        std::visit([&](const auto& arg) {
            std::cout << arg;

            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, float>) {
                if (a.first == "ui_fontsize") {
                    m_fontSize = arg;
                    m_iconFontSize = m_fontSize - 2.0f;
                }
            } else if constexpr (std::is_same_v<T, std::string>) {
                if (a.first == "ui_mainfont_light") {
                    m_iconPath = FilePath::getFilePath("/Assets/EditorParametersAssets/") + std::string(arg);
                }

                if (a.first == "ui_mainfont_bold") {
                    m_iconPathSecond = FilePath::getFilePath("/Assets/EditorParametersAssets/") + std::string(arg);
                }
            } else if constexpr (std::is_same_v<T, int>) {
                if (a.first == "ui_docking") {
                    m_isUIDocked = static_cast<bool>(arg);
                }

                if (a.first == "window_major") {
                    m_major = arg;
                }

                if (a.first == "window_minor") {
                    m_minor = arg;
                }
            }
        }, a.second);
    }
}

std::string EditorManager::getGUIVersion() {
    std::stringstream versionStream;
    versionStream << "#version " << m_major << m_minor << "0";
    version = versionStream.str();
    return this->version;
}

ImGuiIO* EditorManager::getIO() {
    return m_io;
}
