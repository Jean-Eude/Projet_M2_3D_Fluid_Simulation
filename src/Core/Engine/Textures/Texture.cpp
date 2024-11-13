#include <Texture.hpp>

Texture::~Texture() {
    if (texID != 0) {
        glDeleteTextures(1, &texID);
    }
}

void Texture::loadTexture(const std::string& TexPath, TexType type, bool isFlip, GLint mode, GLint interpolation) {
    this->type = type;

    // Ordre important ici !
    this->texPath = TexPath;
    this->isFlip = isFlip;

    switch (this->type) {
        case TEX_1D:
            this->target = GL_TEXTURE_1D;
            break;
        case TEX_2D:
            this->target = GL_TEXTURE_2D;
            break;
        case TEX_3D:
            this->target = GL_TEXTURE_3D;
            break;
        default:
            return;
    }

    this->mode = mode;
    this->interpolation = interpolation;
    GLenum dataType = GL_UNSIGNED_BYTE;

    if (this->texID == 0) {
        // Créer un nouvel ID
        glGenTextures(1, &this->texID);
    }

    glBindTexture(this->target, this->texID);
    // Fin ordre important ici !

    if(this->target == GL_TEXTURE_1D) {
        glTexParameteri(this->target, GL_TEXTURE_WRAP_S, this->mode);
        glTexParameteri(this->target, GL_TEXTURE_MIN_FILTER, this->interpolation);
        glTexParameteri(this->target, GL_TEXTURE_MAG_FILTER, this->interpolation);

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(isFlip);
        unsigned char *data = stbi_load(this->texPath.c_str(), &width, &height, &nrChannels, 0);

        this->width = width;
        this->height = 0;
        this->depth = 0;

        if (data) {
            GLenum format;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB; 
            else if (nrChannels == 4)
                format = GL_RGBA; 
            else {
                //std::cout << "Unsupported number of channels: " << nrChannels << std::endl;
                stbi_image_free(data);
                return; 
            }

            glTexImage1D(this->target, 0, format, width, 0, format, dataType, data);
            glGenerateMipmap(this->target);
        } else {
            //std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    } else if(this->target == GL_TEXTURE_2D) {
        glTexParameteri(this->target, GL_TEXTURE_WRAP_S, this->mode);
        glTexParameteri(this->target, GL_TEXTURE_WRAP_T, this->mode);
        glTexParameteri(this->target, GL_TEXTURE_MIN_FILTER, this->interpolation);
        glTexParameteri(this->target, GL_TEXTURE_MAG_FILTER, this->interpolation);

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(isFlip);
        unsigned char *data = stbi_load(this->texPath.c_str(), &width, &height, &nrChannels, 0);

        this->width = width;
        this->height = height;
        this->depth = 0;

        if (data) {
            GLenum format;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB; 
            else if (nrChannels == 4)
                format = GL_RGBA; 
            else {
                //std::cout << "Unsupported number of channels: " << nrChannels << std::endl;
                stbi_image_free(data);
                return; 
            }

            glTexImage2D(this->target, 0, format, width, height, 0, format, dataType, data);
            glGenerateMipmap(this->target);
        } else {
            //std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    } else if(this->target == GL_TEXTURE_3D) {
        glTexParameteri(this->target, GL_TEXTURE_WRAP_S, this->mode);
        glTexParameteri(this->target, GL_TEXTURE_WRAP_T, this->mode);
        glTexParameteri(this->target, GL_TEXTURE_WRAP_R, this->mode);
        glTexParameteri(this->target, GL_TEXTURE_MIN_FILTER, this->interpolation);
        glTexParameteri(this->target, GL_TEXTURE_MAG_FILTER, this->interpolation);

        int width, height, depth, nrChannels;
        stbi_set_flip_vertically_on_load(isFlip);
        unsigned char *data = stbi_load(this->texPath.c_str(), &width, &height, &nrChannels, 0);

        if (data) {
            GLenum format;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB; 
            else if (nrChannels == 4)
                format = GL_RGBA; 
            else {
                //std::cout << "Unsupported number of channels: " << nrChannels << std::endl;
                stbi_image_free(data);
                return; 
            }

            depth = 1;

            this->width = width;
            this->height = height;
            this->depth = depth;

            glTexImage3D(this->target, 0, format, width, height, depth, 0, format, dataType, data);
            glGenerateMipmap(this->target);
        } else {
            //std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
    }
    
    unbindTexture();
}

void Texture::loadRawTexture(TexType type, TexInternalFormat texInternal, int width, int height, bool isFlip, GLint mode, GLint interpolation) {
    this->type = type;

    // Ordre important ici !
    this->isFlip = isFlip;

    switch (this->type) {
        case TEX_1D:
            this->target = GL_TEXTURE_1D;
            break;
        case TEX_2D:
            this->target = GL_TEXTURE_2D;
            break;
        case TEX_3D:
            this->target = GL_TEXTURE_3D;
            break;
        default:
            return;
    }

    this->mode = mode;
    this->interpolation = interpolation;

    if (this->texID == 0) {
        // Créer un nouvel ID
        glGenTextures(1, &this->texID);
    }

    glBindTexture(this->target, this->texID); 
    // Fin ordre important ici !

    this->width = width;
    this->height = height;
    this->internal = texInternal;
    
    unsigned char* data = NULL;
    // Test
    /*
    data = new unsigned char[width * height * 4]; // RGBA
    for (int i = 0; i < width * height * 4; i += 4) {
        data[i] = 255;     // Rouge
        data[i + 1] = 0;   // Vert
        data[i + 2] = 0;   // Bleu
        data[i + 3] = 255;   // Alpha
    }*/

    GLenum inter;
    if(this->target == GL_TEXTURE_1D) {
        GLenum dataType, format;

        glTexParameteri(this->target, GL_TEXTURE_WRAP_S, this->mode);
        glTexParameteri(this->target, GL_TEXTURE_MIN_FILTER, this->interpolation);
        glTexParameteri(this->target, GL_TEXTURE_MAG_FILTER, this->interpolation);

        if(texInternal == TEX_R32F || texInternal == TEX_RGB32F || texInternal == TEX_RGBA32F) {
            dataType = GL_FLOAT;

            if(texInternal == TEX_R32F) {
                format = GL_RED;
                inter = GL_R32F;
            } else if(texInternal == TEX_RGB32F) {
                format = GL_RGB;
                inter = GL_RGB32F;
            } else if(texInternal == TEX_RGBA32F) {
                format = GL_RGBA;
                inter = GL_RGBA32F;
            }
        }

        if(texInternal == TEX_R32UI || texInternal == TEX_RGB32UI || texInternal == TEX_RGBA32UI) {
            dataType = GL_UNSIGNED_BYTE;

            if(texInternal == TEX_R32UI) {
                format = GL_RED;
                inter = GL_R32UI;
            } else if(texInternal == TEX_RGB32UI) {
                format = GL_RGB;
                inter = GL_RGB32UI;
            } else if(texInternal == TEX_RGBA32UI) {
                format = GL_RGBA;
                inter = GL_RGBA32UI;
            }
        }

        this->format = inter;

        glTexImage1D(this->target, 0, inter, width, 0, format, dataType, data);
        glGenerateMipmap(this->target);
    } else if(this->target == GL_TEXTURE_2D) {
        GLenum dataType, format;

        glTexParameteri(this->target, GL_TEXTURE_WRAP_S, this->mode);
        glTexParameteri(this->target, GL_TEXTURE_WRAP_T, this->mode);
        glTexParameteri(this->target, GL_TEXTURE_MIN_FILTER, this->interpolation);
        glTexParameteri(this->target, GL_TEXTURE_MAG_FILTER, this->interpolation);

        if(texInternal == TEX_R32F || texInternal == TEX_RGB32F || texInternal == TEX_RGBA32F) {
            dataType = GL_FLOAT;

            if(texInternal == TEX_R32F) {
                format = GL_RED;
                inter = GL_R32F;
            } else if(texInternal == TEX_RGB32F) {
                format = GL_RGB;
                inter = GL_RGB32F;
            } else if(texInternal == TEX_RGBA32F) {
                format = GL_RGBA;
                inter = GL_RGBA32F;
            }
        }

        if(texInternal == TEX_R32UI || texInternal == TEX_RGB32UI || texInternal == TEX_RGBA32UI) {
            dataType = GL_UNSIGNED_BYTE;

            if(texInternal == TEX_R32UI) {
                format = GL_RED;
                inter = GL_R32UI;
            } else if(texInternal == TEX_RGB32UI) {
                format = GL_RGB;
                inter = GL_RGB32UI;
            } else if(texInternal == TEX_RGBA32UI) {
                format = GL_RGBA;
                inter = GL_RGBA32UI;
            }
        }

        this->format = inter;

        glTexImage2D(this->target, 0, inter, width, height, 0, format, dataType, data);
    } else if(this->target == GL_TEXTURE_3D) {
        GLenum dataType, format;

        glTexParameteri(this->target, GL_TEXTURE_WRAP_S, this->mode);
        glTexParameteri(this->target, GL_TEXTURE_WRAP_T, this->mode);
        glTexParameteri(this->target, GL_TEXTURE_WRAP_R, this->mode);     
        glTexParameteri(this->target, GL_TEXTURE_MIN_FILTER, this->interpolation);
        glTexParameteri(this->target, GL_TEXTURE_MAG_FILTER, this->interpolation);

        if(texInternal == TEX_R32F || texInternal == TEX_RGB32F || texInternal == TEX_RGBA32F) {
            dataType = GL_FLOAT;

            if(texInternal == TEX_R32F) {
                format = GL_RED;
                inter = GL_R32F;
            } else if(texInternal == TEX_RGB32F) {
                format = GL_RGB;
                inter = GL_RGB32F;
            } else if(texInternal == TEX_RGBA32F) {
                format = GL_RGBA;
                inter = GL_RGBA32F;
            }
        }

        if(texInternal == TEX_R32UI || texInternal == TEX_RGB32UI || texInternal == TEX_RGBA32UI) {
            dataType = GL_UNSIGNED_BYTE;

            if(texInternal == TEX_R32UI) {
                format = GL_RED;
                inter = GL_R32UI;
            } else if(texInternal == TEX_RGB32UI) {
                format = GL_RGB;
                inter = GL_RGB32UI;
            } else if(texInternal == TEX_RGBA32UI) {
                format = GL_RGBA;
                inter = GL_RGBA32UI;
            }
        }

        this->format = inter;

        int depth = 1;
        glTexImage3D(this->target, 0, inter, width, height, depth, 0, format, dataType, data);
        glGenerateMipmap(this->target);
    }

    delete[] data;
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