#include <ComputeShader.hpp>

ComputeShader::~ComputeShader() {
    unloadShader();
}

void ComputeShader::loadComputeShader(const std::string& compute) {
    unloadShader();

    PrintWorkGroupsCapabilities();

    this->m_compute = compute;
    std::string computeCode;
    std::ifstream cShaderFile(this->m_compute);

    cShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        std::stringstream cShadertream;
        cShadertream << cShaderFile.rdbuf();
        cShaderFile.close();
        computeCode = cShadertream.str();
    } catch (std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        return;
    }

    const char* cShaderCode = computeCode.c_str();

    // Compute Shader
    int success;
    char infoLog[512];
    this->m_computeID = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(this->m_computeID, 1, &cShaderCode, NULL);
    glCompileShader(this->m_computeID);
    glGetShaderiv(this->m_computeID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(this->m_computeID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Linking the Shader
    this->m_computeShaderID = glCreateProgram();
    glAttachShader(this->m_computeShaderID, m_computeID);
    glLinkProgram(this->m_computeShaderID);
    glGetProgramiv(this->m_computeShaderID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->m_computeShaderID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(this->m_computeID);
}

void ComputeShader::hotReload() {
    loadComputeShader(this->m_compute.c_str());
}

void ComputeShader::unloadShader() {
    if (this->m_computeShaderID != 0) {
        glDeleteProgram(this->m_computeShaderID);
        this->m_computeShaderID = 0;
    }
}

void ComputeShader::useShader(CS_type type) {
    this->type = type;

    glUseProgram(this->m_computeShaderID);

    // Permet de tout couvrir
    int nX = (int)ceil((float)(this->ElementsX + numGroupsX - 1) / numGroupsX);
    int nY = (int)ceil((float)(this->ElementsY + numGroupsY - 1) / numGroupsY);
    int nZ = (int)ceil((float)(this->ElementsZ + numGroupsZ - 1) / numGroupsZ);

    glDispatchCompute(nX, nY, nZ);

    if(type == CS_TEXTURE) {
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    } else {
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    }
}

unsigned int ComputeShader::getComputeID() {
    return this->m_computeID;
}

unsigned int ComputeShader::getComputeShaderID() {
    return this->m_computeShaderID;
}

std::string& ComputeShader::getName() {
    return m_name;
}

void ComputeShader::setName(const std::string& name) {
    m_name = name;
}

std::string& ComputeShader::getComputePath() {
    return this->m_compute;
}

void ComputeShader::setNumGroups(GLuint numGroupsX, GLuint numGroupsY, GLuint numGroupsZ, int ElementsX, int ElementsY, int ElementsZ) {
    setNumGroupsX(numGroupsX);
    setNumGroupsY(numGroupsY);
    setNumGroupsZ(numGroupsZ);

    this->ElementsX = ElementsX;
    this->ElementsY = ElementsY;
    this->ElementsZ = ElementsZ;
}

GLuint ComputeShader::getNumGroupsX() {
    return this->numGroupsX;
}

GLuint ComputeShader::getNumGroupsY() {
    return this->numGroupsY;
}

GLuint ComputeShader::getNumGroupsZ() {
    return this->numGroupsZ;
}

void ComputeShader::setNumGroupsX(GLuint numGroupsX) {
    this->numGroupsX = numGroupsX;
}

void ComputeShader::setNumGroupsY(GLuint numGroupsY) {
    this->numGroupsY = numGroupsY;
}

void ComputeShader::setNumGroupsZ(GLuint numGroupsZ) {
    this->numGroupsZ = numGroupsZ;
}

int ComputeShader::getElementsX() {
    return this->ElementsX;
}

int ComputeShader::getElementsY() {
    return this->ElementsY;
}

int ComputeShader::getElementsZ() {
    return this->ElementsZ;
}

void ComputeShader::PrintWorkGroupsCapabilities() {
  int workgroup_count[3];
  int workgroup_size[3];
  int workgroup_invocations;

  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &workgroup_count[0]);
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &workgroup_count[1]);
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &workgroup_count[2]);

  printf ("Taille maximale des workgroups:\n\tx:%u\n\ty:%u\n\tz:%u\n",
  workgroup_size[0], workgroup_size[1], workgroup_size[2]);

  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &workgroup_size[0]);
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &workgroup_size[1]);
  glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &workgroup_size[2]);

  printf ("Nombre maximal d'invocation locale:\n\tx:%u\n\ty:%u\n\tz:%u\n",
  workgroup_size[0], workgroup_size[1], workgroup_size[2]);

  glGetIntegerv (GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &workgroup_invocations);
  printf ("Nombre maximum d'invocation de workgroups:\n\t%u\n", workgroup_invocations);
}