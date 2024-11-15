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
unsigned int VBO2, VAO2, EBO2;
unsigned int VBO3, VAO3, EBO3;


int numGroupsX = 256;
int numGroupsY = 1;
int numGroupsZ = 1;


struct Particule {
    glm::vec3 pos;
    glm::vec3 dir;
    float life;
    float maxLife;
};

int nbParticules = 100000;
std::vector<Particule> particles(nbParticules);
std::uniform_real_distribution<float> lifeDist(2.0f, 5.0f); 

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
    

    // Particules 
    shaders.enqueueShader("Particule", FilePath::getFilePath("/Assets/EngineAssets/Shaders/ParticuleVert.glsl"), FilePath::getFilePath("/Assets/EngineAssets/Shaders/ParticuleFrag.glsl"));
    shaders.enqueueComputeShader("ParticuleCS", FilePath::getFilePath("/Assets/EngineAssets/Shaders/ParticuleCS.cs"));
    shaders.setNumGroupsComputeShaderByName("ParticuleCS", numGroupsX, numGroupsY, numGroupsZ, nbParticules, 1, 1);

    shaders.useComputeShaderByName("ParticuleCS", CS_SSBO);    
    shaders.setCompBind3f("ParticuleCS", "gravity", glm::vec3(0.0f, -9.8f, 0.0f));
    shaders.setCompBind3f("ParticuleCS", "spawnPos", glm::vec3(0.0f, 0.0f, 0.0f));
    shaders.setCompBind2f("ParticuleCS", "velocityRange", 1.0f, 3.0f);
    shaders.setCompBind2f("ParticuleCS", "lifeRange", 2.0f, 5.0f);

    std::default_random_engine generator;
    std::uniform_real_distribution<float> positionDist(-0.1f, 0.1f); // Dispersion initiale
    std::uniform_real_distribution<float> velocityDist(1.0f, 3.0f);  // Vitesse initiale
         // Durée de vie aléatoire (en secondes)

    for (auto& p : particles) {
        // Position initiale proche de la source
        p.pos = glm::vec3(positionDist(generator), 0.0f, positionDist(generator));

        // Direction initiale (principalement vers le haut)
        float upwardVelocity = velocityDist(generator);
        p.dir = glm::vec3(positionDist(generator), upwardVelocity, positionDist(generator));

        // Durée de vie
        p.life = lifeDist(generator);
        p.maxLife = p.life;
    }
    

    ssboM.enqueueSSBO("particulesSSBO", GL_DYNAMIC_DRAW, particles);
    
    // Création du VBO pour stocker les positions des particules
    glGenVertexArrays(1, &VAO3);
    glBindVertexArray(VAO3);

    glGenBuffers(1, &VBO3);
    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * nbParticules, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    m_inputs->setMouseScrolledListener([&](const MouseScrolledEvent& e) {
        glm::vec3 cameraDirection = glm::normalize(camera.position);
        camera.position -= 0.075f * e.yOffset * cameraDirection;
        
        float length = glm::length(camera.position);
        if (length < 0.1f) {
            camera.position = 0.1f * cameraDirection;
        }
    });

    GLFWwindow* window = getWindow();
    m_inputs->setMouseButtonPressedListener([&](const MouseButtonPressedEvent& e) {
        glm::vec2 mousePosition = m_inputs->getMousePosition();
        camera.startPos.x = ((mousePosition.x - (m_fbo.getFBOWidth() / 2) ) / (m_fbo.getFBOWidth() / 2)) * camera.radius;
        camera.startPos.y = (((m_fbo.getFBOHeight() / 2) - mousePosition.y) / (m_fbo.getFBOHeight() / 2)) * camera.radius;
        camera.startPos.z = camera.z_axis(camera.startPos.x, camera.startPos.y);
        camera.setFlag(true);
    });

    m_inputs->setMouseButtonReleasedListener([&](const MouseButtonReleasedEvent& e) {
        camera.replace();
        camera.setFlag(false);
    });

    m_inputs->setMouseMovedListener([&](const MouseMovedEvent& e) {
        if (camera.getFlag()) {
            camera.currentPos.x = ((e.mouseX - (m_fbo.getFBOWidth() / 2)) / (m_fbo.getFBOWidth() / 2)) * camera.radius;
            camera.currentPos.y = (((m_fbo.getFBOHeight() / 2) - e.mouseY) / (m_fbo.getFBOHeight() / 2)) * camera.radius;
            camera.currentPos.z = camera.z_axis(camera.currentPos.x, camera.currentPos.y);
            camera.rotation();
        }
    });

}

void EngineManager::OnUpdateWindowEngine() {
    // Inputs
    glfwPollEvents();
    gEventManager.DispatchEvents();
    if (m_inputs->IsKeyPressed(GLFW_KEY_ESCAPE)) {
        m_done = true;
    }

    if (m_inputs->IsKeyPressed(GLFW_KEY_R)) {
        
    }

    m_TimersList.at(0).Update();
    while(m_TimersList.at(0).getAcc() >= m_TimersList.at(0).getMSPerUpdate()) {
        m_TimersList.at(0).UpdateDeltaTime();
    }
    
    // Others
    m_editor.OnUpdateUI();

    float deltaTime = static_cast<float>(m_TimersList.at(0).getDeltaTime());

    ssboM.bindBufferBaseByName("particulesSSBO");
    //glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssboM.getSSBO_IDByName("particulesSSBO"));
    shaders.useComputeShaderByName("ParticuleCS", CS_SSBO);
    shaders.setCompBind1f("ParticuleCS", "deltaTime", deltaTime);

    // Déja fait dans useComputeShaderByName
    //glDispatchCompute((nbParticules + numGroupsX-1) / numGroupsX, 1, 1);
    //glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);


    // Rendu des particules
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(glm::radians(70.0f), static_cast<float>(m_fbo.getFBOWidth()) / m_fbo.getFBOHeight(), 0.1f, 1000.0f);
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, camera.position); 
    view = glm::rotate(view, glm::radians(camera.angle), camera.rotationalAxis);
    glm::mat4 mvp = projection * view * model;

    m_fbo.bindFBO();
        OnUpdateWindow();

        shaders.useShaderByName("Particule");
        shaders.setBind4fv("Particule", "mvp", 1, GL_FALSE, glm::value_ptr(mvp));
        glBindVertexArray(VAO3);
        glPointSize(1.0f);
        glDrawArrays(GL_POINTS, 0, nbParticules);
        glBindVertexArray(0);

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