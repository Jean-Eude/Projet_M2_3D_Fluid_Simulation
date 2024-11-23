#include <EngineManager.hpp>
#include <EventManager.hpp>


void updateCubeAndPlane(float size,  std::vector<glm::vec3>& BoiteVerts) {
    // Boîte
    BoiteVerts.clear();
    BoiteVerts.push_back(glm::vec3(-size, -size, -size));
    BoiteVerts.push_back(glm::vec3(size, -size, -size));
    BoiteVerts.push_back(glm::vec3(size,  size, -size));
    BoiteVerts.push_back(glm::vec3(-size,  size, -size));
    BoiteVerts.push_back(glm::vec3(-size, -size,  size));
    BoiteVerts.push_back(glm::vec3(size, -size,  size));
    BoiteVerts.push_back(glm::vec3(size,  size,  size));
    BoiteVerts.push_back(glm::vec3(-size,  size,  size));

    // Plan
}

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


// Alignement de 16 octets (obligatoire) pour une structure (CS et VS)
struct alignas(16) Particule {
    glm::vec3 pos;       
    float _pad1;
    glm::vec3 velocity;
    float _pad2;
    glm::vec3 dir;
    float _pad3;
    glm::vec3 force;
    float _pad4;
    float scale;         
    float life;
    float density;
    int isActive;
};


int pboxSize = 15;
int nbParticules = pboxSize * pboxSize * pboxSize;
std::vector<Particule> particles(nbParticules);

glm::vec minAABB = glm::vec3(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());
glm::vec maxAABB = glm::vec3(-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity());

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

    const float boxSize = 0.2f;

    // Calcul de smoothLength
    float Volume = boxSize * boxSize * boxSize;
    float Part_Espacement = pow(Volume / nbParticules, 1.0 / 3.0);
    float smoothingLength = 2.0 * Part_Espacement;
    //


    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions         // texture coords
        boxSize,  boxSize, boxSize,  1.0f, 1.0f,  // top right
        boxSize, boxSize, -boxSize,  1.0f, 0.0f,  // bottom right
        -boxSize, boxSize, -boxSize,  0.0f, 0.0f,  // bottom left
        -boxSize,  boxSize, boxSize,  0.0f, 1.0f   // top left 
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

    std::cout << "Size of Particule: " << sizeof(Particule) << std::endl;

    /*
    float boxVertices[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f
    };*/

    std::vector<glm::vec3> boxVertices = {
        glm::vec3(-boxSize, -boxSize, -boxSize),
        glm::vec3(boxSize, -boxSize, -boxSize),
        glm::vec3(boxSize,  boxSize, -boxSize),
        glm::vec3(-boxSize,  boxSize, -boxSize),
        glm::vec3(-boxSize, -boxSize,  boxSize),
        glm::vec3(boxSize, -boxSize,  boxSize),
        glm::vec3(boxSize,  boxSize,  boxSize),
        glm::vec3(-boxSize,  boxSize,  boxSize),
    };

    for (glm::vec3& AABB : boxVertices) {
        minAABB = glm::min(minAABB, AABB);
        maxAABB = glm::max(maxAABB, AABB);
    }

    std::cout << minAABB.x << minAABB.y << minAABB.z << std::endl;
    std::cout << maxAABB.x << maxAABB.y << maxAABB.z << std::endl;

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
    glBufferData(GL_ARRAY_BUFFER, boxVertices.size() * sizeof(glm::vec3), boxVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(boxIndices), boxIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Particules 
    SharedServices::GetInstance().RegisterService("smoothingL", std::make_shared<float>(smoothingLength));

    float restDensity = 1000.f;
    SharedServices::GetInstance().RegisterService("particleRestDensity", std::make_shared<float>(restDensity));

    float mass = 0.02f;
    SharedServices::GetInstance().RegisterService("mass", std::make_shared<float>(mass));

    float viscosity = 1.0f;
    SharedServices::GetInstance().RegisterService("viscosity", std::make_shared<float>(viscosity));

    float stiffness = 1.0f;
    SharedServices::GetInstance().RegisterService("stiffness", std::make_shared<float>(stiffness));

    float tailleParticule = 10.f;
    SharedServices::GetInstance().RegisterService("sizeParti", std::make_shared<float>(tailleParticule));

    float restitution = 0.3f;
    SharedServices::GetInstance().RegisterService("restitution", std::make_shared<float>(restitution));

    SharedServices::GetInstance().RegisterFunction<void, float, std::vector<glm::vec3>&>(
        "updateMesh", std::function<void(float, std::vector<glm::vec3>&)>(updateCubeAndPlane)
    );
    
    shaders.enqueueShader("Particule", FilePath::getFilePath("/Assets/EngineAssets/Shaders/ParticuleVert.glsl"), FilePath::getFilePath("/Assets/EngineAssets/Shaders/ParticuleFrag.glsl"));
    shaders.enqueueComputeShader("particleDensityCS", FilePath::getFilePath("/Assets/EngineAssets/Shaders/particleDensity.cs"));
    shaders.setNumGroupsComputeShaderByName("particleDensityCS", numGroupsX, numGroupsY, numGroupsZ, nbParticules, 1, 1);

    shaders.useComputeShaderByName("particleDensityCS");
    shaders.setCompBind1i("particleDensityCS", "particleCount", nbParticules);
    shaders.setCompBind1f("particleDensityCS", "particleMass", *SharedServices::GetInstance().GetService<float>("mass"));
    shaders.setCompBind1f("particleDensityCS", "smoothingLength", *SharedServices::GetInstance().GetService<float>("smoothingL"));

    std::cout << smoothingLength << std::endl;

    shaders.enqueueComputeShader("particlePhysicsCS", FilePath::getFilePath("/Assets/EngineAssets/Shaders/particlePhysics.cs"));
    shaders.setNumGroupsComputeShaderByName("particlePhysicsCS", numGroupsX, numGroupsY, numGroupsZ, nbParticules, 1, 1);

    shaders.useComputeShaderByName("particlePhysicsCS");
    shaders.setCompBind1i("particlePhysicsCS", "particleCount", nbParticules);

    //std::cout << "smoothing" << smoothingLength << std::endl;

    shaders.enqueueComputeShader("particleIntegrationCS", FilePath::getFilePath("/Assets/EngineAssets/Shaders/particleIntegration.cs"));
    shaders.setNumGroupsComputeShaderByName("particleIntegrationCS", numGroupsX, numGroupsY, numGroupsZ, nbParticules, 1, 1);

    shaders.useComputeShaderByName("particleIntegrationCS");

    shaders.useShaderByName("Particule");    
    shaders.setBind3f("Particule", "minAABB", minAABB);
    shaders.setBind3f("Particule", "maxAABB", maxAABB);

    for (std::size_t i = 0; i < pboxSize; ++i) {
        for (std::size_t j = 0; j < pboxSize; ++j) {
            for (std::size_t k = 0; k < pboxSize; ++k) {
                std::size_t index = i + j * pboxSize + k * pboxSize * pboxSize;

                particles[index].pos = glm::vec3(
                    0.01f * i,
                    0.01f * j,
                    0.01f * k
                );

                particles[index].dir = glm::vec3(0.f, 0.f, 0.f);
                particles[index].velocity = glm::vec3(0.0f, 0.0f, 0.0f);
                particles[index].scale = tailleParticule;
                particles[index].life = 1.;
                particles[index].density = 0.0f;
                particles[index].force = glm::vec3(0.0f, 0.0f, 0.0f);
            }
        }
    }
    //std::cout << particles.size() << std::endl;

    ssboM.enqueueSSBO("particulesSSBO", GL_DYNAMIC_DRAW, particles);
    
    // Création du VBO pour stocker les positions des particules
    glGenVertexArrays(1, &VAO3);
    glBindVertexArray(VAO3);

    glGenBuffers(1, &VBO3);
    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * nbParticules, nullptr, GL_DYNAMIC_DRAW);

    std::cout << sizeof(glm::vec3) * nbParticules << std::endl;

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
        //shaders.useComputeShaderByName("ParticuleCS");
        //shaders.memoryBarrierByName("ParticuleCS", CS_SSBO);

        //shaders.setCompBind3f("ParticuleCS", "minAABB", minAABB);
        //shaders.setCompBind3f("ParticuleCS", "maxAABB", maxAABB);

        shaders.reloadAllShaders();
        shaders.useShaderByName("Particule");
        shaders.setBind3f("Particule", "minAABB", minAABB);
        shaders.setBind3f("Particule", "maxAABB", maxAABB);

        shaders.useShaderByName("Box");
        shaders.useShaderByName("Base");

        m_TimersList.at(0).Pause();
    }

    if (m_inputs->IsKeyPressed(GLFW_KEY_W)) {
        m_TimersList.at(0).Play();
    }

    ssboM.bindBufferBaseByName("particulesSSBO");

    m_TimersList.at(0).Update();
    while(m_TimersList.at(0).getAcc() >= m_TimersList.at(0).getMSPerUpdate()) {

        // MAJ de la logique
        float deltaTime = static_cast<float>(m_TimersList.at(0).getDeltaTime());
        shaders.useComputeShaderByName("particleDensityCS");
        shaders.memoryBarrierByName("particleDensityCS", CS_SSBO);

        /*for (size_t i = 0; i < nbParticules; ++i) {
            std::cout << "Particle " << i << ": " << output[i].density << std::endl;
        }*/

        shaders.useComputeShaderByName("particleDensityCS");
        shaders.setCompBind1f("particleDensityCS", "particleMass", *SharedServices::GetInstance().GetService<float>("mass"));
        shaders.setCompBind1f("particleDensityCS", "smoothingLength", *SharedServices::GetInstance().GetService<float>("smoothingL"));

        shaders.useComputeShaderByName("particlePhysicsCS");
        shaders.setCompBind1f("particlePhysicsCS", "deltaTime", deltaTime);
        shaders.memoryBarrierByName("particlePhysicsCS", CS_SSBO);
        shaders.setCompBind1f("particlePhysicsCS", "particleRestDensity", *SharedServices::GetInstance().GetService<float>("particleRestDensity"));
        shaders.setCompBind1f("particlePhysicsCS", "particleMass", *SharedServices::GetInstance().GetService<float>("mass"));
        shaders.setCompBind1f("particlePhysicsCS", "particleViscosity", *SharedServices::GetInstance().GetService<float>("viscosity"));
        shaders.setCompBind1f("particlePhysicsCS", "stiffness", *SharedServices::GetInstance().GetService<float>("stiffness"));
        shaders.setCompBind1f("particlePhysicsCS", "smoothingLength", *SharedServices::GetInstance().GetService<float>("smoothingL"));

        shaders.useComputeShaderByName("particleIntegrationCS");
        shaders.setCompBind1f("particleIntegrationCS", "deltaTime", deltaTime);
        shaders.setCompBind3f("particleIntegrationCS", "minAABB", minAABB);
        shaders.setCompBind3f("particleIntegrationCS", "maxAABB", maxAABB);
        shaders.setCompBind1f("particleIntegrationCS", "globalTime", m_TimersList.at(0).getTotalTimeinSeconds());
        shaders.setCompBind1f("particleIntegrationCS", "deltaTime", deltaTime);
        shaders.setCompBind3f("particleIntegrationCS", "minAABB", minAABB);
        shaders.setCompBind3f("particleIntegrationCS", "maxAABB", maxAABB);
        shaders.setCompBind3f("particleIntegrationCS", "canonPosition", glm::vec3(0.0f, 0.0f, 0.0f));  
        shaders.setCompBind3f("particleIntegrationCS", "canonDirection", glm::vec3(1.0f, 1.0f, 0.0f)); 
        shaders.setCompBind1f("particleIntegrationCS", "emissionRate", 50.0f);     
        shaders.setCompBind1f("particleIntegrationCS", "particleLifetime", 150.0f);
        shaders.setCompBind1f("particleIntegrationCS", "speed", 200.0f);           
        shaders.setCompBind1f("particleIntegrationCS", "dispersion", 0.1f);     
        shaders.setCompBind1f("particleIntegrationCS", "restitution", *SharedServices::GetInstance().GetService<float>("restitution"));


        shaders.memoryBarrierByName("particleIntegrationCS", CS_SSBO);

        /*std::vector<Particule> output(nbParticules);
        ssboM.linkSSBOByName("particulesSSBO", GL_READ_ONLY, output);

        for (size_t i = 0; i < nbParticules; ++i) {
            output[i].pos += deltaTime * output[i].velocity;
            particles[i] = output[i];
            std::cout << output[i].velocity.x << '\n';
        }
        //particles = output;*/

        m_TimersList.at(0).UpdateDeltaTime();
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

        // Particules
        shaders.useShaderByName("Particule");
        shaders.setBind4fv("Particule", "mvp", 1, GL_FALSE, glm::value_ptr(mvp));
        shaders.setBind1f("Particule", "tailleParticule", *SharedServices::GetInstance().GetService<float>("sizeParti"));
        glPointSize(10.);
        glBindVertexArray(VAO3);
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