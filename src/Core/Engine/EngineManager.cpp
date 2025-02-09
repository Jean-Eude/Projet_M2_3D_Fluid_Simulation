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


glm::vec3 lightPositions = glm::vec3(0.0f, 0.0f, 0.0f);

std::unique_ptr<Models> plane;
std::unique_ptr<Models> box;
std::unique_ptr<Models> mesh;

int numGroupsX = 256;
int numGroupsY = 1;
int numGroupsZ = 1;

unsigned gridSize = 32;
float meshScale = 0.27f;

int pboxSize = 35;
int nbParticules = pboxSize * pboxSize * pboxSize;
std::vector<Particule> particles(nbParticules);
unsigned int VBO3, VAO3, EBO3;


float nearPlane = 0.1f;
float farPlane = 1000.f;
float minDepth = nearPlane;
float maxDepth = farPlane;

static std::shared_ptr<GPUBuffersManager> gpuBuffersManager;

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
    
    SharedServices::GetInstance().RegisterService("fbo", std::make_shared<GPUBuffersManager>(ssboM));
    gpuBuffersManager = SharedServices::GetInstance().GetService<GPUBuffersManager>("fbo");

    // En dessous de m_inputs
    m_editor.OnInitUI(m_window);
    
    addTimer("Global Timer");
    m_TimersList.at(0).Start();

    // Paramètres
    float size = 0.7f;

    // Plan
    shaders.enqueueShader("Base", FilePath::getFilePath("/Assets/EngineAssets/Shaders/PlaneVert.glsl"), FilePath::getFilePath("/Assets/EngineAssets/Shaders/PlaneFrag.glsl"));
    textures.enqueueTexture("terrain", FilePath::getFilePath("/Assets/EngineAssets/Textures/terrain2.jpg"), TEX_2D, true, GL_REPEAT, GL_LINEAR);
    plane = ModelManager::getInstance().createModel("Plane", static_cast<float>(size), 50, true);
    plane->Init();
    shaders.useShaderByName("Base");
    shaders.setBind1i("Base", "tex0", textures.getTextureUnit("terrain"));

    // Boîte
    shaders.enqueueShader("Box", FilePath::getFilePath("/Assets/EngineAssets/Shaders/BoxVert.glsl"), FilePath::getFilePath("/Assets/EngineAssets/Shaders/BoxFrag.glsl"));
    box = ModelManager::getInstance().createModel("Box", static_cast<float>(size) / 2.f);
    box->Init();
    shaders.useShaderByName("Box");
    shaders.setBind1i("Box", "tex0", textures.getTextureUnit("terrain"));

    std::cout << box->getBBmin().x << box->getBBmin().y << box->getBBmin().z << std::endl;
    std::cout << box->getBBmax().x << box->getBBmax().y << box->getBBmax().z << std::endl;

    // Particules
    SharedServices::GetInstance().RegisterService("ScreenSize", std::make_shared<glm::vec2>(glm::vec2(1280, 720)));
    SharedServices::GetInstance().RegisterService("BBmin", std::make_shared<glm::vec3>(box->getBBmin()));
    SharedServices::GetInstance().RegisterService("BBmax", std::make_shared<glm::vec3>(box->getBBmax()));
    glm::vec3 colorParti = glm::vec3(0., 0., 1.);
    SharedServices::GetInstance().RegisterService("CouleurParticule", std::make_shared<glm::vec3>(colorParti));
    float coeffAbsorption = 0.0;
    SharedServices::GetInstance().RegisterService("CoeffAbso", std::make_shared<float>(coeffAbsorption));
    float sigma = 0.1;
    SharedServices::GetInstance().RegisterService("Sigma", std::make_shared<float>(sigma));

    // Mesh
    shaders.enqueueShader("Mesh", FilePath::getFilePath("/Assets/EngineAssets/Shaders/SimpleShadingVert.glsl"), FilePath::getFilePath("/Assets/EngineAssets/Shaders/SimpleShadingFrag.glsl"));
    mesh = ModelManager::getInstance().createModel("Mesh", FilePath::getFilePath("/Assets/EngineAssets/Models/suzanne.obj"));
    mesh->Init();
    shaders.useShaderByName("Mesh");
    shaders.setBind1i("Mesh", "tex0", textures.getTextureUnit("terrain"));

    // Calcul de smoothLength
    float Volume = size * size * size;
    float Part_Espacement = pow(Volume / nbParticules, 1.0 / 3.0);
    float smoothingLength = 2.0 * Part_Espacement;
    SharedServices::GetInstance().RegisterService("SmoothingLength", std::make_shared<float>(smoothingLength));

    float restDensity = 1000.f;
    SharedServices::GetInstance().RegisterService("particleRestDensity", std::make_shared<float>(restDensity));

    float mass = 0.02f;
    SharedServices::GetInstance().RegisterService("mass", std::make_shared<float>(mass));

    float viscosity = 1.0f;
    SharedServices::GetInstance().RegisterService("viscosity", std::make_shared<float>(viscosity));

    float stiffness = 1.0f;
    SharedServices::GetInstance().RegisterService("stiffness", std::make_shared<float>(stiffness));

    float tailleParticule = 20.f;
    SharedServices::GetInstance().RegisterService("sizeParti", std::make_shared<float>(tailleParticule));

    float restitution = 0.3f;
    SharedServices::GetInstance().RegisterService("restitution", std::make_shared<float>(restitution));

    float gravity = 9.81f;
    SharedServices::GetInstance().RegisterService("gravity", std::make_shared<float>(gravity));

    bool gravityFollowCamera = false;
    SharedServices::GetInstance().RegisterService("gravityFollowsCamera", std::make_shared<bool>(gravityFollowCamera));

    bool hasMeshCollision = true;
    SharedServices::GetInstance().RegisterService("hasMeshCollision", std::make_shared<bool>(hasMeshCollision));

    int shadingFormat = 0;
    SharedServices::GetInstance().RegisterService("shadingFormat", std::make_shared<int>(shadingFormat));

    // -------------------------------------------------------- //

    shaders.enqueueShader("Particule", FilePath::getFilePath("/Assets/EngineAssets/Shaders/ParticuleVert.glsl"), FilePath::getFilePath("/Assets/EngineAssets/Shaders/SSR.glsl"));
    shaders.enqueueComputeShader("particleDensityCS", FilePath::getFilePath("/Assets/EngineAssets/Shaders/particleDensity.cs"));
    shaders.setNumGroupsComputeShaderByName("particleDensityCS", numGroupsX, numGroupsY, numGroupsZ, nbParticules, 1, 1);
    shaders.useComputeShaderByName("particleDensityCS");
    shaders.setCompBind1i("particleDensityCS", "particleCount", nbParticules);
    shaders.setCompBind1f("particleDensityCS", "particleMass", *SharedServices::GetInstance().GetService<float>("mass"));
    shaders.setCompBind1f("particleDensityCS", "smoothingLength", *SharedServices::GetInstance().GetService<float>("SmoothingLength"));
    shaders.setCompBind3f("particleDensityCS", "minAABB", *SharedServices::GetInstance().GetService<glm::vec3>("BBmin"));
    shaders.setCompBind3f("particleDensityCS", "maxAABB", *SharedServices::GetInstance().GetService<glm::vec3>("BBmax"));
    shaders.setCompBind1u("particleDensityCS", "gridSize", gridSize);

    shaders.enqueueComputeShader("particlePhysicsCS", FilePath::getFilePath("/Assets/EngineAssets/Shaders/particlePhysics.cs"));
    shaders.setNumGroupsComputeShaderByName("particlePhysicsCS", numGroupsX, numGroupsY, numGroupsZ, nbParticules, 1, 1);
    shaders.useComputeShaderByName("particlePhysicsCS");
    shaders.setCompBind1i("particlePhysicsCS", "particleCount", nbParticules);
    shaders.setCompBind3f("particlePhysicsCS", "minAABB", *SharedServices::GetInstance().GetService<glm::vec3>("BBmin"));
    shaders.setCompBind3f("particlePhysicsCS", "maxAABB", *SharedServices::GetInstance().GetService<glm::vec3>("BBmax"));
    shaders.setCompBind1u("particlePhysicsCS", "gridSize", gridSize);

    shaders.enqueueComputeShader("particleIntegrationCS", FilePath::getFilePath("/Assets/EngineAssets/Shaders/particleIntegration.cs"));
    shaders.setNumGroupsComputeShaderByName("particleIntegrationCS", numGroupsX, numGroupsY, numGroupsZ, nbParticules, 1, 1);
    shaders.useComputeShaderByName("particleIntegrationCS");

    shaders.useShaderByName("Particule");    
    shaders.setBind3f("Particule", "minAABB", *SharedServices::GetInstance().GetService<glm::vec3>("BBmin"));
    shaders.setBind3f("Particule", "maxAABB", *SharedServices::GetInstance().GetService<glm::vec3>("BBmax"));
    shaders.setBind1f("Particule", "nearPlane", nearPlane);
    shaders.setBind1f("Particule", "farPlane", farPlane);
    shaders.setBind2f("Particule", "ScreenSize", *SharedServices::GetInstance().GetService<glm::vec2>("ScreenSize"));
    shaders.setBind1f("Particule", "smoothingLength", *SharedServices::GetInstance().GetService<float>("SmoothingLength"));
    shaders.setBind3f("Particule", "baseColor", *SharedServices::GetInstance().GetService<glm::vec3>("CouleurParticule"));

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

    glGenVertexArrays(1, &VAO3);
    glBindVertexArray(VAO3);

    glGenBuffers(1, &VBO3);
    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * nbParticules, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    ssboM.enqueueSSBO("particulesSSBO", GL_DYNAMIC_DRAW, particles);
    LinkedSpatialHashGrid::initShaders(*this, particles, *SharedServices::GetInstance().GetService<glm::vec3>("BBmin"), *SharedServices::GetInstance().GetService<glm::vec3>("BBmax"), gridSize, numGroupsX, numGroupsY, numGroupsZ);

    std::vector<float> scaledVertices(3 * mesh->getNbVerts() / 8);
    for (std::size_t i = 0; i < mesh->getNbVerts() / 8; ++i) {
        scaledVertices[3 * i] = meshScale * mesh->getVertices()[8 * i];
        scaledVertices[3 * i + 1] = meshScale * mesh->getVertices()[8 * i + 1];
        scaledVertices[3 * i + 2] = meshScale * mesh->getVertices()[8 * i + 2];
    }

    ssboM.enqueueSSBO("vertexSSBO", GL_STATIC_DRAW, scaledVertices);
    ssboM.enqueueSSBO("elementSSBO", GL_STATIC_DRAW, mesh->getIndices());

    // ---------------------------------------------------------------------------- // 
    auto fbom = SharedServices::GetInstance().GetService<GPUBuffersManager>("fbo");

    m_inputs->setMouseScrolledListener([&](const MouseScrolledEvent& e) {
        glm::vec3 cameraDirection = glm::normalize(camera.position);
        camera.position -= 0.075f * e.yOffset * cameraDirection;
        
        float length = glm::length(camera.position);
        if (length < 0.1f) {
            camera.position = 0.1f * cameraDirection;
        }
    });

    m_inputs->setMouseButtonPressedListener([&](const MouseButtonPressedEvent& e) {
        if (e.button == GLFW_MOUSE_BUTTON_LEFT) {
            glm::vec2 mousePosition = m_inputs->getMousePosition();

            glm::vec2 normalizedMousePos = {(mousePosition.x / gpuBuffersManager->getFBODimensionsByName("fbom").first) * 2.0f - 1.0f, -((mousePosition.y / gpuBuffersManager->getFBODimensionsByName("fbom").second) * 2.0f - 1.0f)
            };

            camera.startPos.x = normalizedMousePos.x * camera.radius;
            camera.startPos.y = normalizedMousePos.y * camera.radius;
            camera.startPos.z = camera.z_axis(camera.startPos.x, camera.startPos.y);
            camera.currentPos = camera.startPos;

            camera.setFlag(true);
        }
    });

    m_inputs->setMouseButtonReleasedListener([&](const MouseButtonReleasedEvent& e) {
        if (camera.currentPos != camera.startPos) {
            camera.replace();
        }
        camera.setFlag(false);
    });

    m_inputs->setMouseMovedListener([&](const MouseMovedEvent& e) {
        if (camera.getFlag()) {
            glm::vec2 normalizedMousePos = { (e.mouseX / gpuBuffersManager->getFBODimensionsByName("fbom").first) * 2.0f - 1.0f, -((e.mouseY / gpuBuffersManager->getFBODimensionsByName("fbom").second) * 2.0f - 1.0f) };

            camera.currentPos.x = normalizedMousePos.x * camera.radius;
            camera.currentPos.y = normalizedMousePos.y * camera.radius;
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

    ssboM.bindBufferBaseByName("particulesSSBO");
    LinkedSpatialHashGrid::buildParticlesInteraction(*this, particles, *SharedServices::GetInstance().GetService<glm::vec3>("BBmin"), *SharedServices::GetInstance().GetService<glm::vec3>("BBmax"), gridSize);
    ssboM.bindBufferBaseByName("vertexSSBO");
    ssboM.bindBufferBaseByName("elementSSBO");

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), static_cast<float>(gpuBuffersManager->getFBODimensionsByName("fbom").first) / gpuBuffersManager->getFBODimensionsByName("fbom").second, nearPlane, farPlane);
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, camera.position); 
    view = glm::rotate(view, glm::radians(camera.angle), camera.rotationalAxis);
    glm::mat4 mvp = projection * view * model;

    glm::mat4 meshModel = glm::scale(glm::mat4(1.0f), glm::vec3(meshScale));
    glm::mat4 meshMvp = projection * view * meshModel;

    m_TimersList.at(0).Update();
    while(m_TimersList.at(0).getAcc() >= m_TimersList.at(0).getMSPerUpdate()) {
        float deltaTime = static_cast<float>(m_TimersList.at(0).getDeltaTime());
        shaders.useComputeShaderByName("particleDensityCS");
        shaders.memoryBarrierByName("particleDensityCS", CS_SSBO);

        shaders.useComputeShaderByName("particleDensityCS");
        shaders.setCompBind1f("particleDensityCS", "particleMass", *SharedServices::GetInstance().GetService<float>("mass"));
        shaders.setCompBind1f("particleDensityCS", "smoothingLength", *SharedServices::GetInstance().GetService<float>("SmoothingLength"));

        shaders.useComputeShaderByName("particlePhysicsCS");
        shaders.setCompBind1f("particlePhysicsCS", "deltaTime", deltaTime);
        shaders.memoryBarrierByName("particlePhysicsCS", CS_SSBO);
        shaders.setCompBind1f("particlePhysicsCS", "particleRestDensity", *SharedServices::GetInstance().GetService<float>("particleRestDensity"));
        shaders.setCompBind1f("particlePhysicsCS", "particleMass", *SharedServices::GetInstance().GetService<float>("mass"));
        shaders.setCompBind1f("particlePhysicsCS", "particleViscosity", *SharedServices::GetInstance().GetService<float>("viscosity"));
        shaders.setCompBind1f("particlePhysicsCS", "stiffness", *SharedServices::GetInstance().GetService<float>("stiffness"));
        shaders.setCompBind1f("particlePhysicsCS", "smoothingLength", *SharedServices::GetInstance().GetService<float>("SmoothingLength"));
        if (*SharedServices::GetInstance().GetService<bool>("gravityFollowsCamera")) {
            shaders.setCompBind3f("particlePhysicsCS", "gravity", glm::vec3(glm::inverse(view) * glm::vec4(0.0f, -*SharedServices::GetInstance().GetService<float>("gravity"), 0.0f, 0.0f)));
        }
        else {
            shaders.setCompBind3f("particlePhysicsCS", "gravity", glm::vec3(0.0f, *SharedServices::GetInstance().GetService<float>("gravity"), 0.0f));
        }

        shaders.useComputeShaderByName("particleIntegrationCS");
        shaders.setCompBind1f("particleIntegrationCS", "deltaTime", deltaTime);
        shaders.setCompBind3f("particleIntegrationCS", "minAABB", *SharedServices::GetInstance().GetService<glm::vec3>("BBmin"));
        shaders.setCompBind3f("particleIntegrationCS", "maxAABB", *SharedServices::GetInstance().GetService<glm::vec3>("BBmax"));
        shaders.setCompBind1f("particleIntegrationCS", "globalTime", m_TimersList.at(0).getTotalTimeinSeconds());
        shaders.setCompBind1f("particleIntegrationCS", "deltaTime", deltaTime);
        shaders.setCompBind3f("particleIntegrationCS", "minAABB", *SharedServices::GetInstance().GetService<glm::vec3>("BBmin"));
        shaders.setCompBind3f("particleIntegrationCS", "maxAABB", *SharedServices::GetInstance().GetService<glm::vec3>("BBmax"));
        shaders.setCompBind3f("particleIntegrationCS", "canonPosition", glm::vec3(0.0f, 0.0f, 0.0f));  
        shaders.setCompBind3f("particleIntegrationCS", "canonDirection", glm::vec3(1.0f, 1.0f, 0.0f)); 
        shaders.setCompBind1f("particleIntegrationCS", "emissionRate", 50.0f);     
        shaders.setCompBind1f("particleIntegrationCS", "particleLifetime", 150.0f);
        shaders.setCompBind1f("particleIntegrationCS", "speed", 200.0f);           
        shaders.setCompBind1f("particleIntegrationCS", "dispersion", 0.1f);     
        shaders.setCompBind1f("particleIntegrationCS", "restitution", *SharedServices::GetInstance().GetService<float>("restitution"));
        shaders.setCompBind1u("particleIntegrationCS", "triangleCount", mesh->getNbIndices() / 3);
        shaders.setCompBind1i("particleIntegrationCS", "hasMeshCollision", *SharedServices::GetInstance().GetService<bool>("hasMeshCollision"));

        shaders.memoryBarrierByName("particleIntegrationCS", CS_SSBO);
        m_TimersList.at(0).UpdateDeltaTime();
    }

    // Others
    m_editor.OnUpdateUI();

    SharedServices::GetInstance().GetService<GPUBuffersManager>("fbo")->bindFBO("fbom");
        OnUpdateWindow(); 

        textures.bindAllTextures();
        
        // Plane
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        shaders.useShaderByName("Base");
        shaders.setBind4fv("Base", "mvp", 1, GL_FALSE, glm::value_ptr(mvp));
        plane->Update();
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // Box
        shaders.useShaderByName("Box");
        shaders.setBind4fv("Box", "mvp", 1, GL_FALSE, glm::value_ptr(mvp));
        box->Update();

        // Mesh
        if (*SharedServices::GetInstance().GetService<bool>("hasMeshCollision") == true) {
            shaders.useShaderByName("Mesh");
            shaders.setBind4fv("Mesh", "model", 1, GL_FALSE, glm::value_ptr(meshModel));
            shaders.setBind4fv("Mesh", "mvp", 1, GL_FALSE, glm::value_ptr(meshMvp));
            mesh->Update();
        }

        // Particules
        if(*SharedServices::GetInstance().GetService<int>("shadingFormat") == 0) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ONE);
            shaders.replaceShader("Particule", FilePath::getFilePath("/Assets/EngineAssets/Shaders/ParticuleVert.glsl"), FilePath::getFilePath("/Assets/EngineAssets/Shaders/SSR.glsl"));
        } else {
            shaders.replaceShader("Particule", FilePath::getFilePath("/Assets/EngineAssets/Shaders/ParticuleVert.glsl"), FilePath::getFilePath("/Assets/EngineAssets/Shaders/ParticuleFrag.glsl"));
        }

        shaders.useShaderByName("Particule");
        shaders.setBind4fv("Particule", "mvp", 1, GL_FALSE, glm::value_ptr(mvp));
        shaders.setBind4fv("Particule", "view", 1, GL_FALSE, glm::value_ptr(view));
        shaders.setBind4fv("Particule", "projection", 1, GL_FALSE, glm::value_ptr(projection));
        shaders.setBind1f("Particule", "tailleParticule", *SharedServices::GetInstance().GetService<float>("sizeParti"));
        shaders.setBind3f("Particule", "camPos", camera.position);
        shaders.setBind2f("Particule", "ScreenSize", *SharedServices::GetInstance().GetService<glm::vec2>("ScreenSize"));
        shaders.setBind1f("Particule", "smoothingLength", *SharedServices::GetInstance().GetService<float>("SmoothingLength"));
        shaders.setBind1f("Particule", "nearPlane", nearPlane);
        shaders.setBind1f("Particule", "farPlane", farPlane);
        shaders.setBind3f("Particule", "baseColor", *SharedServices::GetInstance().GetService<glm::vec3>("CouleurParticule"));
        shaders.setBind1f("Particule", "absorptionCoefficient", *SharedServices::GetInstance().GetService<float>("CoeffAbso"));
        shaders.setBind1f("Particule", "sigma", *SharedServices::GetInstance().GetService<float>("Sigma"));
        shaders.setBind1i("Particule", "shadingFormat", *SharedServices::GetInstance().GetService<int>("shadingFormat"));

        for (const auto& particle : particles) {
            glm::vec4 viewPos = view * glm::vec4(particle.pos, 1.0f);
            float depth = -viewPos.z;

            minDepth = std::min(minDepth, depth);
            maxDepth = std::max(maxDepth, depth);
        }

        shaders.setBind1f("Particule", "minDepth", minDepth);
        shaders.setBind1f("Particule", "maxDepth", maxDepth);

        glPointSize(10.);
        glBindVertexArray(VAO3);
        glEnable(GL_POINT_SMOOTH);
        glDrawArrays(GL_POINTS, 0, nbParticules);
        glBindVertexArray(0);
        glDisable(GL_BLEND);

    SharedServices::GetInstance().GetService<GPUBuffersManager>("fbo")->unbindFBO("fbom");

    m_editor.OnRenderUI();
    glfwSwapBuffers(m_window);
}

void EngineManager::OnDestroyWindowEngine() {
    gEventManager.Shutdown();

    plane->Clear();
    box->Clear();
    mesh->Clear();

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