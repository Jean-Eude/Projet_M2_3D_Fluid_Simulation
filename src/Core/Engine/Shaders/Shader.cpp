#include <Shader.hpp>

Shader::~Shader() {
    unloadShader();
}

void Shader::loadShader(const std::string& vertex, const std::string& fragment) {
    unloadShader();

    this->m_vertex = vertex;
    this->m_fragment = fragment;

    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vShaderFile(this->m_vertex);
    std::ifstream fShaderFile(this->m_fragment);

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        //std::cout << this->m_vertex << std::endl;

        std::stringstream vShadertream, fShadertream;
        vShadertream << vShaderFile.rdbuf();
        fShadertream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vertexCode = vShadertream.str();
        fragmentCode = fShadertream.str();
    } catch (std::ifstream::failure& e) {

        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        return;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // Vertex Shader
    this->m_vertexID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(this->m_vertexID, 1, &vShaderCode, NULL);
    glCompileShader(this->m_vertexID);
    int success;    
    char infoLog[512];
    glGetShaderiv(this->m_vertexID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(this->m_vertexID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Fragment Shader
    this->m_fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(this->m_fragmentID, 1, &fShaderCode, NULL);
    glCompileShader(this->m_fragmentID);
    glGetShaderiv(this->m_fragmentID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(this->m_fragmentID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Linking the Shader
    this->m_shaderID = glCreateProgram();
    glAttachShader(this->m_shaderID, m_vertexID);
    glAttachShader(this->m_shaderID, m_fragmentID);
    glLinkProgram(this->m_shaderID);
    glGetProgramiv(this->m_shaderID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->m_shaderID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(this->m_vertexID);
    glDeleteShader(this->m_fragmentID);
}

void Shader::hotReload() {
    loadShader(this->m_vertex.c_str(), this->m_fragment.c_str());
}

void Shader::unloadShader() {
    if (this->m_shaderID != 0) {
        glDeleteProgram(this->m_shaderID);
        this->m_shaderID = 0;
    }
}

void Shader::useShader() {
    glUseProgram(this->m_shaderID);
}

unsigned int Shader::getVertexID() {
    return this->m_vertexID;
}

unsigned int Shader::getFragmentID() {
    return this->m_fragmentID;
}

unsigned int Shader::getShaderID() {
    return this->m_shaderID;
}

std::string& Shader::getName() {
    return m_name;
}

void Shader::setName(const std::string& name) {
    m_name = name;
}

std::string& Shader::getVertexPath() {
    return this->m_vertex;
}

std::string& Shader::getFragmentPath() {
    return this->m_fragment;
}