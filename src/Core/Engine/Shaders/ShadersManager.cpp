#include <ShadersManager.hpp>

void ShadersManager::enqueueShader(const std::string& name, const std::string& vertex, const std::string& fragment) {
    auto shader = std::make_shared<Shader>();
    shader->loadShader(vertex, fragment);
    shader->setName(name);
    shaderQueue.push_back(shader);
}

void ShadersManager::dequeueShader(const std::string& name) {
    auto it = std::remove_if(shaderQueue.begin(), shaderQueue.end(), [&](std::shared_ptr<Shader>& shader) {
        return shader->getName() == name;
    });
    shaderQueue.erase(it, shaderQueue.end());
}

void ShadersManager::dequeueAllShaders() {
    shaderQueue.clear();
}

void ShadersManager::reloadShaderByName(const std::string& name) {
    for (auto& shader : shaderQueue) {
        if (shader->getName() == name) {
            shader->loadShader(shader->getVertexPath(), shader->getFragmentPath());
            break;
        }
    }
}

void ShadersManager::reloadAllShaders() {
    for (auto& shader : shaderQueue) {
        shader->loadShader(shader->getVertexPath(), shader->getFragmentPath());
    }
}

void ShadersManager::hotReloadShaderByName(const std::string& name) {
    for (auto& shader : shaderQueue) {
        if (shader->getName() == name) {
            shader->hotReload();
            break;
        }
    }
}

void ShadersManager::hotReloadAllShaders() {
    for (auto& shader : shaderQueue) {
        shader->hotReload();
    } 
}

void ShadersManager::useShaderByName(const std::string& name) {
    for (auto& shader : shaderQueue) {
        if (shader->getName() == name) {
            shader->useShader();
            break;
        }
    }
}

void ShadersManager::enqueueComputeShader(const std::string& name, const std::string& compute) {
    auto shader = std::make_shared<ComputeShader>();
    shader->loadComputeShader(compute);
    shader->setName(name);
    computeShaderQueue.push_back(shader);
}

void ShadersManager::dequeueComputeShader(const std::string& name) {
    auto it = std::remove_if(computeShaderQueue.begin(), computeShaderQueue.end(), [&](std::shared_ptr<ComputeShader>& shader) {
        return shader->getName() == name;
    });
    computeShaderQueue.erase(it, computeShaderQueue.end());
}

void ShadersManager::dequeueAllComputeShaders() {
    computeShaderQueue.clear();
}

void ShadersManager::reloadComputeShaderByName(const std::string& name) {
    for (auto& shader : computeShaderQueue) {
        if (shader->getName() == name) {
            shader->loadComputeShader(shader->getComputePath());
            break;
        }
    }
}

void ShadersManager::reloadAllComputeShaders() {
    for (auto& shader : computeShaderQueue) {
        shader->loadComputeShader(shader->getComputePath());
    }
}

void ShadersManager::hotReloadComputeShaderByName(const std::string& name) {
    for (auto& shader : computeShaderQueue) {
        if (shader->getName() == name) {
            shader->hotReload();
            break;
        }
    }
}

void ShadersManager::hotReloadAllComputeShaders() {
    for (auto& shader : computeShaderQueue) {
        shader->hotReload();
    } 
}

void ShadersManager::useComputeShaderByName(const std::string& name, GLuint numGroupsX, GLuint numGroupsY, GLuint numGroupsZ) {
    for (auto& shader : computeShaderQueue) {
        if (shader->getName() == name) {
            shader->useShader(numGroupsX, numGroupsY, numGroupsZ);
            std::cout << computeShaderQueue.size() << std::endl;
            break;
        }
    }
}


unsigned int ShadersManager::getShaderIDByName(const std::string& name) {
    for (auto& shader : shaderQueue) {
        if (shader->getName() == name) {
            return shader->getShaderID();
        }
    }
}

void ShadersManager::setBind1f(const std::string& name, const GLchar* VarName, GLfloat v0) {
    for (auto& shader : shaderQueue) {
        if (shader->getName() == name) {
            glUniform1f(glGetUniformLocation(shader->getShaderID(), VarName), v0);
            break;
        }
    }
}

void ShadersManager::setBind1i(const std::string& name, const GLchar* VarName, GLint v0) {
    for (auto& shader : shaderQueue) {
        if (shader->getName() == name) {
            glUniform1i(glGetUniformLocation(shader->getShaderID(), VarName), v0);
            break;
        }
    }
}

void ShadersManager::setBind2f(const std::string& name, const GLchar* VarName, GLfloat v0, GLfloat v1) {
    for (auto& shader : shaderQueue) {
        if (shader->getName() == name) {
            glUniform2f(glGetUniformLocation(shader->getShaderID(), VarName), v0, v1);
            break;
        }
    }
}

void ShadersManager::setBind2f(const std::string& name, const GLchar* VarName, glm::vec2 v) {
    for (auto& shader : shaderQueue) {
        if (shader->getName() == name) {
            glUniform2f(glGetUniformLocation(shader->getShaderID(), VarName), v.x, v.y);
            break;
        }
    }
}

void ShadersManager::setBind3f(const std::string& name, const GLchar* VarName, GLfloat v0, GLfloat v1, GLfloat v2) {
    for (auto& shader : shaderQueue) {
        if (shader->getName() == name) {
            glUniform3f(glGetUniformLocation(shader->getShaderID(), VarName), v0, v1, v2);
            break;
        }
    }
}

void ShadersManager::setBind3f(const std::string& name, const GLchar* VarName, glm::vec3 v) {
    for (auto& shader : shaderQueue) {
        if (shader->getName() == name) {
            glUniform3f(glGetUniformLocation(shader->getShaderID(), VarName), v.x, v.y, v.z);
            break;
        }
    }
}

void ShadersManager::setBind4f(const std::string& name, const GLchar* VarName, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
    for (auto& shader : shaderQueue) {
        if (shader->getName() == name) {
            glUniform4f(glGetUniformLocation(shader->getShaderID(), VarName), v0, v1, v2, v3);
            break;
        }
    }
}

void ShadersManager::setBind4f(const std::string& name, const GLchar* VarName, glm::vec4 v) {
    for (auto& shader : shaderQueue) {
        if (shader->getName() == name) {
            glUniform4f(glGetUniformLocation(shader->getShaderID(), VarName), v.x, v.y, v.z, v.w);
            break;
        }
    }
}

void ShadersManager::setBind4fv(const std::string& name, const GLchar* VarName, GLsizei count, GLboolean transpose, const GLfloat *value) {
    for (auto& shader : shaderQueue) {
        if (shader->getName() == name) {
            glUniformMatrix4fv(glGetUniformLocation(shader->getShaderID(), VarName), count, transpose, value);
            break;
        }
    }
}


void ShadersManager::setCompBind1f(const std::string& name, const GLchar* VarName, GLfloat v0) {
    for (auto& shader : computeShaderQueue) {
        if (shader->getName() == name) {
            glUniform1f(glGetUniformLocation(shader->getComputeID(), VarName), v0);
            break;
        }
    }
}

void ShadersManager::setCompBind1i(const std::string& name, const GLchar* VarName, GLint v0) {
    for (auto& shader : computeShaderQueue) {
        if (shader->getName() == name) {
            glUniform1i(glGetUniformLocation(shader->getComputeID(), VarName), v0);
            break;
        }
    }
}

void ShadersManager::setCompBind2f(const std::string& name, const GLchar* VarName, GLfloat v0, GLfloat v1) {
    for (auto& shader : computeShaderQueue) {
        if (shader->getName() == name) {
            glUniform2f(glGetUniformLocation(shader->getComputeID(), VarName), v0, v1);
            break;
        }
    }
}

void ShadersManager::setCompBind2f(const std::string& name, const GLchar* VarName, glm::vec2 v) {
    for (auto& shader : computeShaderQueue) {
        if (shader->getName() == name) {
            glUniform2f(glGetUniformLocation(shader->getComputeID(), VarName), v.x, v.y);
            break;
        }
    }
}

void ShadersManager::setCompBind3f(const std::string& name, const GLchar* VarName, GLfloat v0, GLfloat v1, GLfloat v2) {
    for (auto& shader : computeShaderQueue) {
        if (shader->getName() == name) {
            glUniform3f(glGetUniformLocation(shader->getComputeID(), VarName), v0, v1, v2);
            break;
        }
    }
}

void ShadersManager::setCompBind3f(const std::string& name, const GLchar* VarName, glm::vec3 v) {
    for (auto& shader : computeShaderQueue) {
        if (shader->getName() == name) {
            glUniform3f(glGetUniformLocation(shader->getComputeID(), VarName), v.x, v.y, v.z);
            break;
        }
    }
}

void ShadersManager::setCompBind4f(const std::string& name, const GLchar* VarName, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
    for (auto& shader : computeShaderQueue) {
        if (shader->getName() == name) {
            glUniform4f(glGetUniformLocation(shader->getComputeID(), VarName), v0, v1, v2, v3);
            break;
        }
    }
}

void ShadersManager::setCompBind4f(const std::string& name, const GLchar* VarName, glm::vec4 v) {
    for (auto& shader : computeShaderQueue) {
        if (shader->getName() == name) {
            glUniform4f(glGetUniformLocation(shader->getComputeID(), VarName), v.x, v.y, v.z, v.w);
            break;
        }
    }
}

void ShadersManager::setCompBind4fv(const std::string& name, const GLchar* VarName, GLsizei count, GLboolean transpose, const GLfloat *value) {
    for (auto& shader : computeShaderQueue) {
        if (shader->getName() == name) {
            glUniformMatrix4fv(glGetUniformLocation(shader->getComputeID(), VarName), count, transpose, value);
            break;
        }
    }
}