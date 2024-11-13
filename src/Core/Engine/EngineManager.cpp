#include <EngineManager.hpp>
#include <EventManager.hpp>

GLFWwindow* Window::m_window;
GLFWimage* Window::m_icon;
std::string Window::m_iconPath;
GLFWmonitor* Window::m_ecran;
const GLFWvidmode* Window::m_windowParams;
int Window::m_width;
int Window::m_height;
bool Window::m_vsync;


unsigned int VBO, VAO, EBO;
Camera camera;

int numGroupsX = (unsigned int)1280/10;
int numGroupsY = (unsigned int)720/10;
int numGroupsZ = 1;

EngineManager::EngineManager() {
    // Valeurs de base au cas où le fichier de config ne fonctionnerait pass
    m_done = false;
    m_vsync = false;
    m_width = 640;
    m_height = 640;
    m_title = "Flux"; 
    m_major = 4;
    m_minor = 6;
}

void EngineManager::OnReadConfigFile(const char* cfg) {
    readConfigFile(cfg);
    
    for (const auto& a : m_cVars) {
        std::cout << a.first << " : ";

        std::visit([&](const auto& arg) {
            std::cout << arg;

            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, int>) {
                if (a.first == "window_width") {
                    m_width = arg;
                }

                if (a.first == "window_height") {
                    m_height = arg;
                } 

                if (a.first == "window_major") {
                    m_major = static_cast<unsigned int>(arg);
                } 

                if (a.first == "window_minor") {
                    m_minor = static_cast<unsigned int>(arg);
                }

                if (a.first == "window_vsync") {
                    m_vsync = static_cast<bool>(arg);
                }

            } else if constexpr (std::is_same_v<T, std::string>) {
                if (a.first == "window_title") {
                    m_title = arg;
                }
            }
        }, a.second);

        std::cout << std::endl;
    }
}

void EngineManager::OnInitWindowEngine() {
    OnReadConfigFile(FilePath::getFilePath("/Assets/EngineParameters/config_engine.flx").c_str());    
    OnInitWindow(m_width, m_height, m_title, m_major, m_minor, m_vsync);
    m_inputs = std::make_unique<Inputs>(m_window);
    
    // En dessous de m_inputs
    m_editor.OnInitUI(m_window);
    
    addTimer("Global Timer");
    m_TimersList.at(0).Start();
    
    shaders.enqueueShader("Base", FilePath::getFilePath("/Assets/EngineAssets/Shaders/vertex.glsl"), FilePath::getFilePath("/Assets/EngineAssets/Shaders/fragCS.glsl"));

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions         // texture coords
        0.5f,  0.5f, 0.0f,  1.0f, 1.0f,  // top right
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f   // top left 
    };

    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    glBindVertexArray(0); 


    textures.enqueueTexture("terrain1", FilePath::getFilePath("/Assets/EngineAssets/Textures/terrain.jpg"), TEX_2D, true, GL_REPEAT, GL_LINEAR);
    textures.enqueueRawTexture("terrain2", TEX_2D, TEX_RGBA32F, 512, 512, true, GL_REPEAT, GL_LINEAR);
    textures.enqueueTexture("terrain3", FilePath::getFilePath("/Assets/EngineAssets/Textures/terrain2.jpg"), TEX_2D, true, GL_REPEAT, GL_LINEAR);

    shaders.useShaderByName("Base");
    shaders.setBind1i("Base", "tex0", textures.getTextureUnit("terrain1"));
    shaders.setBind1i("Base", "tex1", textures.getTextureUnit("terrain2"));
    shaders.setBind1i("Base", "tex2", textures.getTextureUnit("terrain3"));

}

void EngineManager::OnUpdateWindowEngine() {
    // Inputs
    glfwPollEvents();
    gEventManager.DispatchEvents();
    if (m_inputs->IsKeyPressed(GLFW_KEY_ESCAPE)) {
        m_done = true;
    }

    if (m_inputs->IsKeyPressed(GLFW_KEY_R)) {
        shaders.hotReloadAllShaders();
        shaders.useShaderByName("Base");
        shaders.setBind1i("Base", "tex0", textures.getTextureUnit("terrain1"));
        shaders.setBind1i("Base", "tex1", textures.getTextureUnit("terrain2"));
        shaders.setBind1i("Base", "tex2", textures.getTextureUnit("terrain3"));
    }

    m_TimersList.at(0).Update();
    while(m_TimersList.at(0).getAcc() >= m_TimersList.at(0).getMSPerUpdate()) {
        m_TimersList.at(0).UpdateDeltaTime();
    }
    
    // Others
    m_editor.OnUpdateUI();

    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(90.0f), static_cast<float>(m_fbo.getFBOWidth()) / m_fbo.getFBOHeight(), 0.1f, 10000.0f);

    glm::vec3 cameraPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, up);
    
    //glm::mat4 view = glm::mat4(1.0f);
    //view = glm::translate(view, camera.position); 
    //view = glm::rotate(view, glm::radians(camera.angle), camera.rotationalAxis);

    glm::mat4 mvp = projection * view * model;
    
    shaders.setBind4fv("Base", "mvp", 1, GL_FALSE, glm::value_ptr(mvp));

    m_fbo.bindFBO();
        OnUpdateWindow();

        textures.bindAllTextures();
        shaders.useShaderByName("Base");

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    m_fbo.unbindFBO();
    
    m_editor.OnRenderUI();
    glfwSwapBuffers(m_window);
}

void EngineManager::OnDestroyWindowEngine() {
    gEventManager.Shutdown();

    m_editor.OnDestroyUI();
        
    OnDestroyWindow();
}

GLFWwindow* EngineManager::getWindow() {
    return m_window;
}

bool EngineManager::getDone() {
    return this->m_done;
}

void EngineManager::setDone(bool newDone) {
    this->m_done = newDone;
}