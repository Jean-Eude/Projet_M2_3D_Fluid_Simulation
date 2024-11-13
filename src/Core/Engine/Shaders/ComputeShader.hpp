#pragma once

#include <HeadersBase.hpp>

enum CS_type {
    CS_TEXTURE,
    CS_SSBO,
};

class ComputeShader {
    public:
        ComputeShader() = default;
        ~ComputeShader();

        void loadComputeShader(const std::string& compute);
        void hotReload();
        void unloadShader();

        void useShader(CS_type type);

        unsigned int getComputeID();
        unsigned int getComputeShaderID();
        std::string& getName();
        void setName(const std::string& name);

        std::string& getComputePath();

        void setNumGroups(GLuint numGroupsX, GLuint numGroupsY, GLuint numGroupsZ, int ElementsX, int ElementsY, int ElementsZ);

        GLuint getNumGroupsX();
        GLuint getNumGroupsY();
        GLuint getNumGroupsZ();

        void setNumGroupsX(GLuint numGroupsX);
        void setNumGroupsY(GLuint numGroupsY);
        void setNumGroupsZ(GLuint numGroupsZ);

        int getWidth();
        int getHeight();
        int getDepth();

        int getElementsX();
        int getElementsY();
        int getElementsZ();

    private:
        std::string m_compute;
        unsigned int m_computeID;
        unsigned int m_computeShaderID;
        std::string m_name;

        // Pour les workgroups
        GLuint numGroupsX, numGroupsY, numGroupsZ;
        int ElementsX, ElementsY, ElementsZ;

        CS_type type;

        void PrintWorkGroupsCapabilities();
};