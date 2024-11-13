#include <EngineManager.hpp>
#include <EventManager.hpp>
#include <random>  // Pour l'initialisation aléatoire

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
Camera camera;
bool flag = false;

int numGroupsX = 16;
int numGroupsY = 16;
int numGroupsZ = 1;

struct Particle {
    glm::vec4 position;
    glm::vec4 velocity;
    glm::vec4 color;
};

const int numParticles = 1000;
std::vector<Particle> particles(numParticles);
std::vector<Particle> outputData2;

void initParticles() {
    // Initialisation du générateur aléatoire et de la distribution
    std::random_device rd;
    std::default_random_engine gen(rd());
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f); // Valeurs entre -1 et 1 pour la position

    // Initialisation des particules
    for (auto& p : particles) {
        // Position aléatoire entre -1.0 et 1.0 pour x, y et z, et une durée de vie fixe de 5.0f pour w
        p.position = glm::vec4(dis(gen), dis(gen), dis(gen), 5.0f); 
        
        // Vitesse aléatoire pour x, y et z, avec une valeur fixe pour w
        p.velocity = glm::vec4(dis(gen), dis(gen), dis(gen), 0.1f); 
        
        // Couleur blanche
        p.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); 
    }
}


EngineManager::EngineManager() {
    // Valeurs de base au cas où le fichier de config ne fonctionnerait pas
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
    
<<<<<<< HEAD
    shaders.enqueueShader("Base", FilePath::getFilePath("/Assets/EngineAssets/Shaders/vertex.glsl"), FilePath::getFilePath("/Assets/EngineAssets/Shaders/fragCS.glsl"));
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
=======
    shaders.enqueueShader("Base", FilePath::getFilePath("/Assets/EngineAssets/Shaders/ParticuleVert.glsl"), FilePath::getFilePath("/Assets/EngineAssets/Shaders/ParticuleFrag.glsl"));
    shaders.enqueueComputeShader("BaseC", FilePath::getFilePath("/Assets/EngineAssets/Shaders/ParticleCS.cs"));

    initParticles();
>>>>>>> f3cbc49 (Sauvegarde temporaire de EngineManager.cpp)

    // Création et allocation du SSBO
    ssboM.enqueueSSBO("ssbo", GL_DYNAMIC_COPY, particles);
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
}

<<<<<<< HEAD
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

    /*m_inputs->setKeyPressedListener([&](const KeyPressedEvent& e) {
        std::cout << e.ToString() << '\n';
    });
    m_inputs->setKeyReleasedListener([&](const KeyReleasedEvent& e) {
        std::cout << e.ToString() << '\n';
    });
    m_inputs->setMouseMovedListener([&](const MouseMovedEvent& e) {
        std::cout << e.ToString() << '\n';
    });
    m_inputs->setMouseScrolledListener([&](const MouseScrolledEvent& e) {
        std::cout << e.ToString() << '\n';
    });
    m_inputs->setMouseButtonPressedListener([&](const MouseButtonPressedEvent& e) {
        std::cout << e.ToString() << '\n';
    });
    m_inputs->setMouseButtonReleasedListener([&](const MouseButtonReleasedEvent& e) {
        std::cout << e.ToString() << '\n';
    });*/

    m_inputs->setMouseScrolledListener([&](const MouseScrolledEvent& e) {
        camera.position -= glm::normalize(camera.position) * 0.075f * e.yOffset;
    });

    GLFWwindow* window = getWindow();
    m_inputs->setMouseButtonPressedListener([&](const MouseButtonPressedEvent& e) {
        glm::vec2 mousePosition = m_inputs->getMousePosition();
        camera.startPos.x = ((mousePosition.x - (m_fbo.getFBOWidth() / 2) ) / (m_fbo.getFBOWidth() / 2)) * camera.radius;
        camera.startPos.y = (((m_fbo.getFBOHeight() / 2) - mousePosition.y) / (m_fbo.getFBOHeight() / 2)) * camera.radius;
        camera.startPos.z = camera.z_axis(camera.startPos.x, camera.startPos.y);
        flag = true;
    });

    m_inputs->setMouseButtonReleasedListener([&](const MouseButtonReleasedEvent& e) {
        camera.replace();
        flag = false;
    });

    m_inputs->setMouseMovedListener([&](const MouseMovedEvent& e) {
        if (flag) {
            camera.currentPos.x = ((e.mouseX - (m_fbo.getFBOWidth() / 2)) / (m_fbo.getFBOWidth() / 2)) * camera.radius;
            camera.currentPos.y = (((m_fbo.getFBOHeight() / 2) - e.mouseY) / (m_fbo.getFBOHeight() / 2)) * camera.radius;
            camera.currentPos.z = camera.z_axis(camera.currentPos.x, camera.currentPos.y);
            camera.rotation();
        }
    });

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

=======
void printParticles(const std::vector<Particle>& particles) {
    for (size_t i = 0; i < particles.size(); ++i) {
        const Particle& p = particles[i];
        std::cout << "Particle " << i << ":\n"
                  << "  Position: (" << p.position.x << ", " << p.position.y << ", " << p.position.z << ", " << p.position.w << ")\n"
                  << "  Velocity: (" << p.velocity.x << ", " << p.velocity.y << ", " << p.velocity.z << ", " << p.velocity.w << ")\n"
                  << "  Color:    (" << p.color.r << ", " << p.color.g << ", " << p.color.b << ", " << p.color.a << ")\n"
                  << std::endl;
    }
>>>>>>> f3cbc49 (Sauvegarde temporaire de EngineManager.cpp)
}

void EngineManager::OnUpdateWindowEngine() {
    // Inputs
    glfwPollEvents();
    gEventManager.DispatchEvents();
    if (m_inputs->IsKeyPressed(GLFW_KEY_ESCAPE)) {
        m_done = true;
    }

    if (m_inputs->IsKeyPressed(GLFW_KEY_R)) {
        ssboM.linkSSBOByName("ssbo", GL_MAP_READ_BIT, outputData2);


        // (Assurez-vous que la donnée de la position est mise à jour correctement)
        // Affichez les positions des particules pour vérification
        for (size_t i = 0; i < outputData2.size(); ++i) {
            std::cout << "Particle " << i << " Position: "
                    << outputData2[i].position.x << ", "
                    << outputData2[i].position.y << ", "
                    << outputData2[i].position.z << std::endl;
        }
    }

    m_TimersList.at(0).Update();
    while(m_TimersList.at(0).getAcc() >= m_TimersList.at(0).getMSPerUpdate()) {
        m_TimersList.at(0).UpdateDeltaTime();
    }
    
    // Mise à jour de l'interface utilisateur
    m_editor.OnUpdateUI();

    // Matrices de transformation
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
<<<<<<< HEAD
    projection = glm::perspective(glm::radians(70.0f), static_cast<float>(m_fbo.getFBOWidth()) / m_fbo.getFBOHeight(), 0.1f, 10000.0f);
    
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, camera.position); 
    view = glm::rotate(view, glm::radians(camera.angle), camera.rotationalAxis);
=======
    projection = glm::perspective(glm::radians(90.0f), static_cast<float>(m_fbo.getFBOWidth()) / m_fbo.getFBOHeight(), 0.1f, 10000.0f);
    glm::vec3 cameraPos = glm::vec3(0.25f, 0.25f, 0.25f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, up);
>>>>>>> f3cbc49 (Sauvegarde temporaire de EngineManager.cpp)

    glm::mat4 mvp = projection * view * model;

    // Bind FBO et lancement du compute shader
    m_fbo.bindFBO();
        OnUpdateWindow();

<<<<<<< HEAD
        textures.bindAllTextures();

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
=======
        shaders.setNumGroupsComputeShaderByName("BaseC", numGroupsX, numGroupsY, numGroupsZ, numParticles, 1, 1);
        shaders.useComputeShaderByName("BaseC", CS_SSBO);
        shaders.setCompBind1f("BaseC", "deltaTime", m_TimersList.at(0).getDeltaTime());
>>>>>>> f3cbc49 (Sauvegarde temporaire de EngineManager.cpp)

        ssboM.linkSSBOByName("ssbo", GL_MAP_READ_BIT, outputData2);
        shaders.useShaderByName("Base");
        
        glBindVertexArray(VAO);
<<<<<<< HEAD
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
=======
        glDrawArrays(GL_POINTS, 0, numParticles);  // Rendu des particules
>>>>>>> f3cbc49 (Sauvegarde temporaire de EngineManager.cpp)
    m_fbo.unbindFBO();

    
    
    // Rendu de l'interface utilisateur
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
