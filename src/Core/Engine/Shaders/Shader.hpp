#pragma once

#include <HeadersBase.hpp>

class Shader {
    public:
        Shader() = default;
        ~Shader();

        void loadShader(const std::string& vertex, const std::string& fragment);
        void unloadShader();

        unsigned int getVertexID() const;
        unsigned int getFragmentID() const;
        unsigned int getShaderID() const;
        const std::string& getName() const;
        void setName(const std::string& name);

    private:
        std::string m_vertex;
        std::string m_fragment;

        unsigned int m_vertexID;
        unsigned int m_fragmentID;
        unsigned int m_shaderID;

        std::string m_name;
};