#include <TexturesManager.hpp>

void TexturesManager::enqueueTexture(const std::string& name, const std::string& TexPath, TexType type, bool isFlip, GLint mode, GLint interpolation) {
    for (const auto& pair : textureQueue) {
        if (pair.first->getName() == name) {
            return; 
        }
    }

    auto texture = std::make_shared<Texture>();
    texture->loadTexture(TexPath, type, isFlip, mode, interpolation);
    texture->setName(name);

    if (nextUnit < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS) {
        textureQueue[texture] = nextUnit;
        nextUnit++;
    } else {
        //std::cerr << "Maximum texture units reached. Unable to add texture: " << name << std::endl;
    }
}

void TexturesManager::enqueueRawTexture(const std::string& name, TexType type, TexInternalFormat texInternal, bool isCompute, int width, int height, bool isFlip, GLint mode, GLint interpolation) {
    for (const auto& pair : textureQueue) {
        if (pair.first->getName() == name) {
            return; 
        }
    }

    auto texture = std::make_shared<Texture>();
    texture->loadRawTexture(type, texInternal, isCompute, 0, width, height, isFlip, mode, interpolation);
    texture->setName(name);

    if (nextUnit < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS) {
        textureQueue[texture] = nextUnit;
        nextUnit++;
    } else {
        //std::cerr << "Maximum texture units reached. Unable to add texture: " << name << std::endl;
    }
}

void TexturesManager::dequeueTexture(const std::string& name) {
    for (auto it = textureQueue.begin(); it != textureQueue.end(); ) {
        if (it->first->getName() == name) {
            it = textureQueue.erase(it);
        } else {
            it++;
        }
    }
}

void TexturesManager::dequeueAllTextures() {
    textureQueue.clear();
    nextUnit = 0;
}

void TexturesManager::reloadTextureByName(const std::string& name) {
    for (auto& pair : textureQueue) {
        if (pair.first->getName() == name) {
            pair.first->loadTexture(pair.first->getPath(), pair.first->getType(), pair.first->getIsFlip(), pair.first->getMode(), pair.first->getInterpolation());
            break;
        }
    }
}

void TexturesManager::reloadAllTextures() {
    for (auto& pair : textureQueue) {
        pair.first->loadTexture(pair.first->getPath(), pair.first->getType(), pair.first->getIsFlip(), pair.first->getMode(), pair.first->getInterpolation());
    }
}

void TexturesManager::reloadRawTextureByName(const std::string& name) {
    for (auto& pair : textureQueue) {
        if (pair.first->getName() == name) {
            pair.first->loadRawTexture(pair.first->getType(), pair.first->getInternalFormat(), pair.first->getIsFlip(), pair.second, pair.first->getWidth(), pair.first->getHeight(), pair.first->getIsFlip(), pair.first->getMode(), pair.first->getInterpolation());
            break;
        }
    }
}

void TexturesManager::reloadAllRawTextures() {
    for (auto& pair : textureQueue) {
        pair.first->loadRawTexture(pair.first->getType(), pair.first->getInternalFormat(), pair.first->getIsFlip(), pair.second, pair.first->getWidth(), pair.first->getHeight(), pair.first->getIsFlip(), pair.first->getMode(), pair.first->getInterpolation());
    }
}

unsigned int TexturesManager::getTextureIDByName(const std::string& name) {
    for (auto& pair : textureQueue) {
        if (pair.first->getName() == name) {
            return pair.first->getTexID();
        }
    }
}

unsigned int TexturesManager::getTextureUnit(const std::string& name) {
    for (auto& pair : textureQueue) {
        if (pair.first->getName() == name) {
            return pair.second;
        }
    }
}

void TexturesManager::bindAllTextures() {
    for (auto& pair : textureQueue) {
        pair.first->bindTexture(pair.second, pair.first->getTexID());
    }
}

void TexturesManager::bindTextureByName(const std::string& name) {
    for (auto& pair : textureQueue) {
        if (pair.first->getName() == name) {
            pair.first->bindTexture(pair.second, pair.first->getTexID());
            break;
        }
    }
}

void TexturesManager::unbindAllTextures() {
    for (auto& pair : textureQueue) {
        pair.first->unbindTexture();
    }
}

void TexturesManager::unbindTextureByName(const std::string& name) {
    for (auto& pair : textureQueue) {
        if (pair.first->getName() == name) {
            pair.first->unbindTexture();
            break;
        }
    }
}