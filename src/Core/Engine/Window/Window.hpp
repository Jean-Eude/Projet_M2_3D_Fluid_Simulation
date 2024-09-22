#pragma once

#include <LogsManager.hpp>
#include <FilePath.hpp>

class Window : public LogsManager {
    protected:
        virtual ~Window() = default;

        void OnInitWindow(int m_width, int m_height, std::string m_title, unsigned int m_major, unsigned int m_minor, bool m_vsync);
        void OnUpdateWindow();
        void OnDestroyWindow();

        std::vector<std::string> getAvailableMonitors();
        
        // Fenêtre
        static GLFWwindow* m_window;

        //Icône
        static GLFWimage* m_icon;
        static std::string m_iconPath;

        // Propriétés de l'écran
        static GLFWmonitor* m_ecran;
        static const GLFWvidmode* m_windowParams;
        
        // Variables
        static int m_width, m_height;
        static bool m_vsync;
    
    public:
        // Setters
        void setWidth(int width);
        void setHeight(int height);
        void setVsync(bool newVsync);
        void setFullscreen();
        void setWindowed();
        void setMonitor(GLFWmonitor* monitor);
        GLFWwindow* getWindow();

        int getWidth();
        int getHeight();
};