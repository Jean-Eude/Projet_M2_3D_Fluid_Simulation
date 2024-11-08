#pragma once

#include <HeadersBase.hpp>

class ComputeShader {
    public:
        ComputeShader() = default;
        ~ComputeShader();

        void loadComputeShader(const std::string& compute);
        void hotReload();
        void unloadShader();
        void useShader(GLuint numGroupsX, GLuint numGroupsY, GLuint numGroupsZ);

        unsigned int getComputeID();
        unsigned int getComputeShaderID();
        std::string& getName();
        void setName(const std::string& name);

        std::string& getComputePath();

    private:
        std::string m_compute;
        unsigned int m_computeID;
        unsigned int m_computeShaderID;
        std::string m_name;

        void PrintWorkGroupsCapabilities();
};