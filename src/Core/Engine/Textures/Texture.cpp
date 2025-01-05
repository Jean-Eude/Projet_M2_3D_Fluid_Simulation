#include <Texture.hpp>

Texture::~Texture() {
    if (texID != 0) {
        glDeleteTextures(1, &texID);
    }
}

void Texture::loadTexture(const std::string& TexPath, TexType type, bool isFlip, GLint mode, GLint interpolation) {
    this->type = type;
    this->texPath = TexPath;
    this->isFlip = isFlip;

    switch (type) {
        case TEX_1D: target = GL_TEXTURE_1D; break;
        case TEX_2D: target = GL_TEXTURE_2D; break;
        case TEX_3D: target = GL_TEXTURE_3D; break;
        case TEX_CUBEMAP: target = GL_TEXTURE_CUBE_MAP; break;
        default:
            std::cerr << "Unsupported texture type!" << std::endl;
            return;
    }

    if (texID == 0) {
        glGenTextures(1, &texID);
    }
    glBindTexture(target, texID);

    glTexParameteri(target, GL_TEXTURE_WRAP_S, mode);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, interpolation);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, interpolation);
    if (target == GL_TEXTURE_2D || target == GL_TEXTURE_3D || target == GL_TEXTURE_CUBE_MAP) {
        glTexParameteri(target, GL_TEXTURE_WRAP_T, mode);
    }
    if (target == GL_TEXTURE_3D || target == GL_TEXTURE_CUBE_MAP) {
        glTexParameteri(target, GL_TEXTURE_WRAP_R, mode);
    }

    if (type == TEX_CUBEMAP) {
        std::vector<std::string> faces;
        std::stringstream ss(TexPath);
        std::string facePath;
        while (std::getline(ss, facePath, '|')) {
            faces.push_back(facePath);
        }

        if (faces.size() != 6) {
            std::cerr << "Cubemap requires exactly 6 textures!" << std::endl;
            return;
        }

        for (unsigned int i = 0; i < faces.size(); ++i) {
            stbi_set_flip_vertically_on_load(false);
            int width, height, nrChannels;
            void* data = nullptr;
            GLenum dataType;
            
            if (stbi_is_hdr(faces[i].c_str())) {
                data = stbi_loadf(faces[i].c_str(), &width, &height, &nrChannels, 0);
                dataType = GL_FLOAT;
            } else {
                data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
                dataType = GL_UNSIGNED_BYTE;
            }

            if (data) {
                GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
                GLenum internalFormat = (dataType == GL_FLOAT) ? 
                                        ((nrChannels == 3) ? GL_RGB16F : GL_RGBA16F) : 
                                        ((nrChannels == 3) ? GL_RGB8 : GL_RGBA8);

                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format, dataType, data);
                stbi_image_free(data);
            } else {
                std::cerr << "Failed to load cubemap texture at: " << faces[i] << std::endl;
                stbi_image_free(data);
                return;
            }
        }

        glGenerateMipmap(target);
        return;
    }

    int width, height, nrChannels;
    void* data = nullptr;
    GLenum dataType;

    stbi_set_flip_vertically_on_load(isFlip);

    if (stbi_is_hdr(TexPath.c_str())) {
        data = stbi_loadf(TexPath.c_str(), &width, &height, &nrChannels, 0);
        dataType = GL_FLOAT;
    } else {
        data = stbi_load(TexPath.c_str(), &width, &height, &nrChannels, 0);
        dataType = GL_UNSIGNED_BYTE;
    }

    if (data) {
        GLenum format, internalFormat;
        switch (nrChannels) {
            case 1: format = GL_RED; internalFormat = (dataType == GL_FLOAT) ? GL_R16F : GL_R8; break;
            case 3: format = GL_RGB; internalFormat = (dataType == GL_FLOAT) ? GL_RGB16F : GL_RGB8; break;
            case 4: format = GL_RGBA; internalFormat = (dataType == GL_FLOAT) ? GL_RGBA16F : GL_RGBA8; break;
            default:
                std::cerr << "Unsupported number of channels: " << nrChannels << std::endl;
                stbi_image_free(data);
                return;
        }

        if (target == GL_TEXTURE_1D) {
            glTexImage1D(target, 0, internalFormat, width, 0, format, dataType, data);
        } else if (target == GL_TEXTURE_2D) {
            glTexImage2D(target, 0, internalFormat, width, height, 0, format, dataType, data);
        } else if (target == GL_TEXTURE_3D) {
            glTexImage3D(target, 0, internalFormat, width, height, depth, 0, format, dataType, data);
        }

        glGenerateMipmap(target);

        this->width = width;
        this->height = height;
        this->depth = 1;
    } else {
        std::cerr << "Failed to load texture: " << TexPath << std::endl;
    }

    stbi_image_free(data);
    unbindTexture();
}

void Texture::loadRawTexture(TexType type, TexInternalFormat texInternal, int width, int height, bool isFlip, GLint mode, GLint interpolation) {
    this->type = type;
    this->width = width;
    this->height = height;
    this->depth = depth;
    this->internal = texInternal;

    switch (type) {
        case TEX_1D: target = GL_TEXTURE_1D; break;
        case TEX_2D: target = GL_TEXTURE_2D; break;
        case TEX_3D: target = GL_TEXTURE_3D; break;
        case TEX_CUBEMAP: target = GL_TEXTURE_CUBE_MAP; break;
        default: 
            std::cerr << "Unsupported texture type for raw loading!" << std::endl; 
            return;
    }

    if (texID == 0) {
        glGenTextures(1, &texID);
    }

    glBindTexture(target, texID);

    glTexParameteri(target, GL_TEXTURE_WRAP_S, mode);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, interpolation);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, interpolation);

    if (target == GL_TEXTURE_2D || target == GL_TEXTURE_3D || target == GL_TEXTURE_CUBE_MAP) {
        glTexParameteri(target, GL_TEXTURE_WRAP_T, mode);
    }
    if (target == GL_TEXTURE_3D || target == GL_TEXTURE_CUBE_MAP) {
        glTexParameteri(target, GL_TEXTURE_WRAP_R, mode);
    }

    GLenum format, dataType;

    switch (texInternal) {
        case TEX_R16F: format = GL_RED; dataType = GL_FLOAT; break;
        case TEX_RGB16F: format = GL_RGB; dataType = GL_FLOAT; break;
        case TEX_RGBA16F: format = GL_RGBA; dataType = GL_FLOAT; break;
        case TEX_R32F: format = GL_RED; dataType = GL_FLOAT; break;
        case TEX_RGB32F: format = GL_RGB; dataType = GL_FLOAT; break;
        case TEX_RGBA32F: format = GL_RGBA; dataType = GL_FLOAT; break;
        case TEX_R32UI: format = GL_RED; dataType = GL_UNSIGNED_INT; break;
        case TEX_RGB32UI: format = GL_RGB; dataType = GL_UNSIGNED_INT; break;
        case TEX_RGBA32UI: format = GL_RGBA; dataType = GL_UNSIGNED_INT; break;
        default:
            std::cerr << "Unsupported internal format for raw texture!" << std::endl;
            return;
    }

    if (target == GL_TEXTURE_1D) {
        glTexImage1D(target, 0, texInternal, width, 0, format, dataType, nullptr);
    } else if (target == GL_TEXTURE_2D) {
        glTexImage2D(target, 0, texInternal, width, height, 0, format, dataType, nullptr);
    } else if (target == GL_TEXTURE_3D) {
        glTexImage3D(target, 0, texInternal, width, height, depth, 0, format, dataType, nullptr);
    } else if (target == GL_TEXTURE_CUBE_MAP) {
        for (unsigned int i = 0; i < 6; ++i) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, texInternal, width, height, 0, format, dataType, nullptr);
        }
    }

    glGenerateMipmap(target);
    unbindTexture();
}

void Texture::unbindTexture() {
    glBindTexture(this->target, 0);
}

void Texture::bindTexture(unsigned int unitTex, GLuint texID) {
    glActiveTexture(GL_TEXTURE0 + unitTex);
    glBindTexture(this->target, texID);
}

void Texture::bindRawTexture(unsigned int unitBinding, unsigned int texID, int level, bool isLayered, int layer, GLenum access) {
    this->level = level;
    this->isLayered = isLayered;
    this->layer = layer;
    this->access = access;

    glBindImageTexture(unitBinding, texID, level, isLayered, layer, access, this->format);
}

GLuint Texture::getTexID() {
    return this->texID;
}

std::string Texture::getPath() {
    return this->texPath;
}

TexType Texture::getType() {
    return this->type;
}

bool Texture::getIsFlip() {
    return this->isFlip;
}

GLenum Texture::getTarget() {
    return this->target;
}

GLint Texture::getMode() {
    return this->mode;
}

GLint Texture::getInterpolation() {
    return this->interpolation;
}

std::string& Texture::getName() {
    return m_name;
}

void Texture::setName(const std::string& name) {
    m_name = name;
}

TexInternalFormat Texture::getInternalFormat() {
    return this->internal;
}

int Texture::getWidth() {
    return this->width;
}

int Texture::getHeight() {
    return this->height;
}

int Texture::getDepth() {
    return this->depth;
}

void Texture::setWidth(int width) {
    this->width = width;
}

void Texture::setHeight(int height) {
    this->height = height;
}

void Texture::setDepth(int depth) {
    this->depth = depth;
}