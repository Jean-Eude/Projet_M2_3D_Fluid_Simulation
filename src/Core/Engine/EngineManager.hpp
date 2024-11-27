#pragma once

#include <ParserConfig.hpp>
#include <Window.hpp>
#include <Inputs.hpp>
#include <FilePath.hpp>
#include <Editor.hpp>
#include <TimerManager.hpp>
#include <GPUBuffersManager.hpp>
#include <ShadersManager.hpp>
#include <TexturesManager.hpp>
#include <Texture.hpp>
#include <Shader.hpp>
#include <ComputeShader.hpp>
#include <SSBO.hpp>
#include <Camera.hpp>
#include <SharedServices.hpp>
#include <ModelManager.hpp>
#include <Models.hpp>

class EngineManager : public ParserConfig, public Window, public TimerManager, public GPUBuffersManager {
    public:
        EngineManager();
        ~EngineManager() = default;

        void OnReadConfigFile(const char* cfg);

        // Window
        void OnInitWindowEngine();
        void OnUpdateWindowEngine();
        void OnDestroyWindowEngine();

        GLFWwindow* getWindow();
        bool getDone();
        void setDone(bool newDone);

        // Inputs
        std::unique_ptr<Inputs> m_inputs;

        // UI
        Editor m_editor;

        // Variables
        bool m_done;
        unsigned int m_major, m_minor;
        std::string m_title;

        // Shader
        Shader shader;
        ShadersManager shaders;
        Texture tex;
        Texture tex1;
        ComputeShader cs;

        TexturesManager textures;

        SSBO ssbo;
        GPUBuffersManager ssboM;

        Camera camera;
};