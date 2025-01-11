#pragma once

#include <Shader.hpp>
#include <ComputeShader.hpp>
#include <ThreadManager.hpp>


enum ShaderType {
    SHADER_VF,
    SHADER_CS,
};

class ShadersManager : public ThreadManager {
    private:
        std::vector<std::shared_ptr<Shader>> shaderQueue;
        std::vector<std::shared_ptr<ComputeShader>> computeShaderQueue;

        std::mutex shaderQueueMutex;

    public:
        ShadersManager() = default;
        ~ShadersManager() = default;

        // Shaders VF
        void enqueueShader(const std::string& name, const std::string& vertex, const std::string& fragment);
        void dequeueShader(const std::string& name);
        void dequeueAllShaders();
        void reloadShaderByName(const std::string& name);
        void reloadAllShaders();
        void hotReloadShaderByName(const std::string& name);
        void hotReloadAllShaders();
        void useShaderByName(const std::string& name);
        void replaceShader(const std::string& name, const std::string& vertex, const std::string& fragment);

        // Shaders C
        void enqueueComputeShader(const std::string& name, const std::string& compute);
        void dequeueComputeShader(const std::string& name);
        void dequeueAllComputeShaders();
        void reloadComputeShaderByName(const std::string& name);
        void reloadAllComputeShaders();
        void hotReloadComputeShaderByName(const std::string& name);
        void hotReloadAllComputeShaders();
        void useComputeShaderByName(const std::string& name); 
        void memoryBarrierByName(const std::string& name, CS_type type); 

        void setNumGroupsComputeShaderByName(const std::string& name, GLuint numGroupsX, GLuint numGroupsY, GLuint numGroupsZ, int ElementsX, int ElementsY, int ElementsZ); 
        void setNumGroupsAllComputeShader(GLuint numGroupsX, GLuint numGroupsY, GLuint numGroupsZ, int ElementsX, int ElementsY, int ElementsZ); 

        int getNumGroupsXComputeShaderByName(const std::string& name); 
        int getNumGroupsYComputeShaderByName(const std::string& name); 
        int getNumGroupsZComputeShaderByName(const std::string& name); 

        // Général
        unsigned int getShaderIDByName(const std::string& name); 
        unsigned int getComputeShaderIDByName(const std::string& name); 

        // Shaders VF
        void setBind1f(const std::string& name, const GLchar* VarName, GLfloat v0);
        void setBind1i(const std::string& name, const GLchar* VarName, GLint v0);
        void setBind1u(const std::string& name, const GLchar* VarName, GLuint v0);
        void setBind2f(const std::string& name, const GLchar* VarName, GLfloat v0, GLfloat v1);
        void setBind2f(const std::string& name, const GLchar* VarName, glm::vec2 v);
        void setBind3f(const std::string& name, const GLchar* VarName, GLfloat v0, GLfloat v1, GLfloat v2);
        void setBind3f(const std::string& name, const GLchar* VarName, glm::vec3 v);
        void setBind3fv(const std::string& name, const GLchar* VarName, GLsizei count, GLboolean transpose, const GLfloat *value);
        void setBind4f(const std::string& name, const GLchar* VarName, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
        void setBind4f(const std::string& name, const GLchar* VarName, glm::vec4 v);
        void setBind4fv(const std::string& name, const GLchar* VarName, GLsizei count, GLboolean transpose, const GLfloat *value);

        // Shaders C
        void setCompBind1f(const std::string& name, const GLchar* VarName, GLfloat v0);
        void setCompBind1i(const std::string& name, const GLchar* VarName, GLint v0);
        void setCompBind1u(const std::string& name, const GLchar* VarName, GLuint v0);
        void setCompBind2f(const std::string& name, const GLchar* VarName, GLfloat v0, GLfloat v1);
        void setCompBind2f(const std::string& name, const GLchar* VarName, glm::vec2 v);
        void setCompBind3f(const std::string& name, const GLchar* VarName, GLfloat v0, GLfloat v1, GLfloat v2);
        void setCompBind3f(const std::string& name, const GLchar* VarName, glm::vec3 v);
        void setCompBind3fv(const std::string& name, const GLchar* VarName, GLsizei count, GLboolean transpose, const GLfloat *value);
        void setCompBind4f(const std::string& name, const GLchar* VarName, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
        void setCompBind4f(const std::string& name, const GLchar* VarName, glm::vec4 v);
        void setCompBind4fv(const std::string& name, const GLchar* VarName, GLsizei count, GLboolean transpose, const GLfloat *value);
};