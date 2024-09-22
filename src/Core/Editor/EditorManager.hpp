#pragma once

#include <HeadersBase.hpp>
#include <ParserConfig.hpp>
#include <FilePath.hpp>

class EditorManager : public ParserConfig {
    protected:
        virtual ~EditorManager() = default;

        ImGuiIO* m_io = nullptr;
        GLFWwindow* m_window_ui = nullptr;
        ImFontConfig* m_icon_config = new ImFontConfig();
        ImWchar* m_icon_range = new ImWchar[3];
        ImFont* m_font = nullptr;
        std::string m_iconPath;
        std::string m_iconPathSecond;

        float m_fontSize;
        float m_iconFontSize;

        bool m_isUIDocked;

        virtual void OnReadConfigFile(const char* cfg);
};