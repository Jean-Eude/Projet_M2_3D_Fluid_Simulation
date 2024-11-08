#pragma once

#include <HeadersBase.hpp>

class Shader {
    public:
        Shader() = default;
        ~Shader();

        void loadShader(const std::string& vertex, const std::string& fragment);
        void hotReload();
        void unloadShader();
        void useShader();

        unsigned int getVertexID();
        unsigned int getFragmentID();
        unsigned int getShaderID();
        std::string& getName();
        void setName(const std::string& name);

        std::string& getVertexPath();
        std::string& getFragmentPath();

    private:
        std::string m_vertex;
        std::string m_fragment;

        unsigned int m_vertexID;
        unsigned int m_fragmentID;
        unsigned int m_shaderID;

        std::string m_name;
};