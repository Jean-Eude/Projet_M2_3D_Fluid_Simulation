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
    glm::vec3 velocity;
    float scale;
    float life;
};

int nbParticules = 100000;
std::vector<Particule> particles(nbParticules);

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

    // Plan
    shaders.enqueueShader("Base", FilePath::getFilePath("/Assets/EngineAssets/Shaders/vertex.glsl"), FilePath::getFilePath("/Assets/EngineAssets/Shaders/frag.glsl"));
    shaders.enqueueShader("Box", FilePath::getFilePath("/Assets/EngineAssets/Shaders/boxVertex.glsl"), FilePath::getFilePath("/Assets/EngineAssets/Shaders/boxFragment.glsl"));

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions         // texture coords
        0.5f,  0.5f, 0.5f,  1.0f, 1.0f,  // top right
        0.5f, 0.5f, -0.5f,  1.0f, 0.0f,  // bottom right
        -0.5f, 0.5f, -0.5f,  0.0f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.5f,  0.0f, 1.0f   // top left 
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

    textures.enqueueTexture("terrain", FilePath::getFilePath("/Assets/EngineAssets/Textures/terrain2.jpg"), TEX_2D, true, GL_REPEAT, GL_LINEAR);

    shaders.useShaderByName("Base");
    shaders.setBind1i("Base", "tex0", textures.getTextureUnit("terrain"));

    // Boîte
    shaders.useShaderByName("Box");

    float boxVertices[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f
    };

    unsigned int boxIndices[] = {
        0, 1,
        1, 2,
        2, 3,
        3, 0,
        4, 5,
        5, 6,
        6, 7,
        7, 4,
        0, 4,
        1, 5,
        2, 6,
        3, 7
    };

    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &EBO2);

    glBindVertexArray(VAO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(boxVertices), boxVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(boxIndices), boxIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Particules 
    shaders.enqueueShader("Particule", FilePath::getFilePath("/Assets/EngineAssets/Shaders/ParticuleVert.glsl"), FilePath::getFilePath("/Assets/EngineAssets/Shaders/ParticuleFrag.glsl"));
    shaders.enqueueComputeShader("ParticuleCS", FilePath::getFilePath("/Assets/EngineAssets/Shaders/ParticuleCS.cs"));
    shaders.setNumGroupsComputeShaderByName("ParticuleCS", numGroupsX, numGroupsY, numGroupsZ, nbParticules, 1, 1);

    shaders.useComputeShaderByName("ParticuleCS", CS_SSBO);    


    for (auto& p : particles) {
        p.pos = glm::vec3(0., 0., 0.);
        p.dir = glm::vec3(0., 0., 0.);
        p.velocity = glm::vec3(0., 0., 0.);
        p.scale = 1.;
        p.life = 1.;
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
        shaders.hotReloadAllComputeShaders();
        shaders.useComputeShaderByName("ParticuleCS", CS_SSBO);

        shaders.reloadAllShaders();
        shaders.useShaderByName("Particule");
        shaders.useShaderByName("Box");
        shaders.useShaderByName("Base");

        m_TimersList.at(0).Pause();
    }

    if (m_inputs->IsKeyPressed(GLFW_KEY_W)) {
        m_TimersList.at(0).Play();
    }

    m_TimersList.at(0).Update();
    while(m_TimersList.at(0).getAcc() >= m_TimersList.at(0).getMSPerUpdate()) {

        // MAJ de la logique
        float deltaTime = static_cast<float>(m_TimersList.at(0).getDeltaTime());
        ssboM.bindBufferBaseByName("particulesSSBO");
        shaders.useComputeShaderByName("ParticuleCS", CS_SSBO);
        shaders.setCompBind1f("ParticuleCS", "deltaTime", deltaTime);
        shaders.setCompBind3f("ParticuleCS", "gravity", glm::vec3(0.0f, 9.8f, 0.0f));
        shaders.setCompBind3f("ParticuleCS", "spawnPos", glm::vec3(0.0f, 0.0f, 0.0f));
        shaders.setCompBind3f("ParticuleCS", "initialVelocity", glm::vec3(0.0f, -25.0f, 0.0f));
        shaders.setCompBind1f("ParticuleCS", "lifeSpan", 10.0f);

        m_TimersList.at(0).UpdateDeltaTime();
    }

    std::vector<Particule> debugParticles(nbParticules);

    // Mapper le SSBO pour lire les données
    ssboM.linkSSBOByName("particulesSSBO", GL_READ_ONLY, debugParticles);

    // Afficher les particules
    if (debugParticles.size() > 50) {
        const auto& p = debugParticles[50]; // Index 50 (51ᵉ particule)

        // Condition : afficher seulement si la particule vient d'être lancée
        if (p.life == 15 && p.life > 0) { // Elle vient d'être réinitialisée
            std::cout << "Particle 50 launched: Position = (" << p.pos.x << ", " << p.pos.y << ", " << p.pos.z << ")"
                    << ", Velocity = (" << p.velocity.x << ", " << p.velocity.y << ", " << p.velocity.z << ")"
                    << ", Life = " << p.life << std::endl;
        }
    } else {
        std::cerr << "Error: Less than 51 particles exist." << std::endl;
    }
    
    // Others
    m_editor.OnUpdateUI();

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

        textures.bindAllTextures();

        // Particules
        shaders.useShaderByName("Particule");
        shaders.setBind4fv("Particule", "mvp", 1, GL_FALSE, glm::value_ptr(mvp));
        glBindVertexArray(VAO3);
        glPointSize(5.0f);
        glDrawArrays(GL_POINTS, 0, nbParticules);
        glBindVertexArray(0);

        // Box
        shaders.useShaderByName("Box");
        shaders.setBind4fv("Box", "mvp", 1, GL_FALSE, glm::value_ptr(mvp));
        glBindVertexArray(VAO2);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBindVertexArray(0);

        // Base
        shaders.useShaderByName("Base");
        shaders.setBind4fv("Base", "mvp", 1, GL_FALSE, glm::value_ptr(mvp));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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