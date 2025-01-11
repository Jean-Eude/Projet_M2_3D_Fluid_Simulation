#include <Window.hpp>

void Window::OnInitWindow(int m_width, int m_height, std::string m_title, unsigned int m_major, unsigned int m_minor, bool m_vsync) {
    // Initialisation de GLFW
    if (!glfwInit()) {
        LogsManager::enqueueConsoleLogsInit("GLFW n'a pas été initialisé correctement !", LogLevel::ERROR);
        exit(1); 
    } else {
        LogsManager::enqueueConsoleLogsInit("GLFW a été initialisé correctement !", LogLevel::OK);
    }

    // Spécification des attributs de la fenêtre
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE); 
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
    
    // Création de la fenêtre GLFW
    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    if (!m_window) {
        LogsManager::enqueueConsoleLogsInit("La fenêtre GLFW n'a pas été créée !", LogLevel::ERROR);
        glfwTerminate();
        exit(1);    
    } else {
        LogsManager::enqueueConsoleLogsInit("La fenêtre GLFW a été créée !", LogLevel::OK);
    }

    // Obtention du moniteur principal
    m_ecran = glfwGetPrimaryMonitor();
    if (!m_ecran) {
        LogsManager::enqueueConsoleLogsInit("Impossible d'obtenir le moniteur principal !", LogLevel::ERROR);
        glfwDestroyWindow(m_window);
        glfwTerminate();
        exit(1);
    } else {
        LogsManager::enqueueConsoleLogsInit("Obtention du moniteur principal réussi !", LogLevel::OK);
    }

    // Obtention du mode vidéo du moniteur principal
    m_windowParams = glfwGetVideoMode(m_ecran);
    if (!m_windowParams) {
        LogsManager::enqueueConsoleLogsInit("Impossible d'obtenir le mode vidéo du moniteur principal !", LogLevel::ERROR);
        glfwDestroyWindow(m_window);
        glfwTerminate();
        exit(1);
    } else {
        LogsManager::enqueueConsoleLogsInit("Obtention du mode vidéo du moniteur principal réussi !", LogLevel::OK);
    }

    // Définition du contexte OpenGL actuel
    glfwMakeContextCurrent(m_window);

    // Vsync
    glfwSwapInterval(m_vsync);
    
    // Setup de l'icône
    m_icon = new GLFWimage;
    m_iconPath = FilePath::getFilePath("/Assets/EngineParametersAssets/Icon128x128.png");
    if (!(m_icon->pixels = stbi_load(m_iconPath.c_str(), &m_icon->width, &m_icon->height, 0, 4))) {
        LogsManager::enqueueConsoleLogsInit("Échec du chargement de l'icône !", LogLevel::ERROR);
        LogsManager::enqueueConsoleLogsInit(std::string(m_iconPath), LogLevel::DEBUG);
    } else {
        glfwSetWindowIcon(m_window, 1, m_icon);
        LogsManager::enqueueConsoleLogsInit("Réussite du chargement de l'icône !", LogLevel::OK);
        LogsManager::enqueueConsoleLogsInit(std::string(m_iconPath), LogLevel::DEBUG);
    }

    // Initialisation de GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LogsManager::enqueueConsoleLogsInit("GLAD n'a pas été initialisé correctement !", LogLevel::ERROR);
        glfwDestroyWindow(m_window);
        glfwTerminate();
        exit(1);
    } else {
        LogsManager::enqueueConsoleLogsInit("GLAD a été initialisé correctement !", LogLevel::OK);
    }

    glEnable(GL_DEPTH_TEST);

    //glDepthFunc(GL_LESS);
    //glEnable(GL_CULL_FACE);

    // Pour modifier la taille des points dans les shaders
    glEnable(GL_PROGRAM_POINT_SIZE);
}

void Window::OnUpdateWindow() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
}

void Window::OnDestroyWindow() {
    if (m_icon) {
        if (m_icon->pixels) {
            stbi_image_free(m_icon->pixels);
        }
        delete m_icon;
        m_icon = nullptr;
    }

    if (m_window) {
        glfwSetWindowShouldClose(m_window, GLFW_TRUE);
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    LogsManager::enqueueConsoleLogsInit("Le programme a correctement été fermé !", LogLevel::OK);
}

GLFWwindow* Window::getWindow() {
    return m_window;
}

void Window::setWidth(int width) { 
    m_width = width;
    glfwSetWindowSize(m_window, m_width, m_height);

}

void Window::setHeight(int height) { 
    m_height = height; 
    glfwSetWindowSize(m_window, m_width, m_height);
}

void Window::setVsync(bool newVsync) {
    glfwSwapInterval(newVsync);
}

void Window::setFullscreen() {
    glfwSetWindowMonitor(m_window, m_ecran, 0, 0, m_windowParams->width, m_windowParams->height, m_windowParams->refreshRate);

    m_width = m_windowParams->width;
    m_height = m_windowParams->height;
}

void Window::setWindowed() {
    int windowWidth = m_width;
    int windowHeight = m_height;
    int screenWidth = m_windowParams->width;
    int screenHeight = m_windowParams->height;
    int xpos = (screenWidth - windowWidth) / 2;
    int ypos = (screenHeight - windowHeight) / 2;
    glfwSetWindowMonitor(m_window, NULL, m_width / 4, m_height / 4, m_width, m_height, m_windowParams->refreshRate);
}

std::vector<std::string> Window::getAvailableMonitors() {
    int count;
    GLFWmonitor** monitors = glfwGetMonitors(&count);

    std::vector<std::string> monitorNames;
    for (int i = 0; i < count; i++) {
        const char* name = glfwGetMonitorName(monitors[i]);
        if (name) {
            monitorNames.push_back(std::string(name));
        }
    }

    return monitorNames;
}

void Window::setMonitor(GLFWmonitor* monitor) {
    if (monitor == NULL) {
        std::cerr << "Monitor is NULL." << std::endl;
        return;
    }
    
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    if (mode == NULL) {
        std::cerr << "Failed to get video mode for the monitor." << std::endl;
        return;
    }
    
    glfwSetWindowMonitor(glfwGetCurrentContext(), monitor, 0, 0, m_width, m_height, mode->refreshRate);
}

int Window::getWidth() {
    return m_width;
}

int Window::getHeight() {
    return m_height;
}