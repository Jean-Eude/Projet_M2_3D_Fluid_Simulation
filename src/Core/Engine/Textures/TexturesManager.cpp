#include <TexturesManager.hpp>

void TexturesManager::enqueueTexture(const std::string& name, const std::string& TexPath, TexType type, bool isFlip, GLint mode, GLint interpolation) {
    if (textureQueue.find(name) != textureQueue.end()) {
        return;
    }

    auto texture = std::make_shared<Texture>();
    texture->loadTexture(TexPath, type, isFlip, mode, interpolation);
    texture->setName(name);

    if (nextUnit < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS) {
        textureQueue[name] = texture;
        textureUnits[name] = nextUnit;
        textureCompBindingUnits[name] = nextUnitComp;
        nextUnit++;
    } else {
        std::cerr << "Maximum texture units reached. Unable to add texture: " << name << std::endl;
    }
}

void TexturesManager::enqueueRawTexture(const std::string& name, TexType type, TexInternalFormat texInternal, int width, int height, bool isFlip, GLint mode, GLint interpolation) {
    if (textureQueue.find(name) != textureQueue.end()) {
        return;
    }

    auto texture = std::make_shared<Texture>();
    texture->loadRawTexture(type, texInternal, width, height, isFlip, mode, interpolation);
    texture->setName(name);

    if (nextUnit < GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS) {
        // SI il ya un pb avec plusieurs textures, ça peut venir d'ici
        //nextUnitComp = 0;
        textureQueue[name] = texture;
        textureUnits[name] = nextUnit;
        textureCompBindingUnits[name] = nextUnitComp;
        nextUnit++;
        nextUnitComp++;
    } else {
        std::cerr << "Maximum texture units reached. Unable to add texture: " << name << std::endl;
    }
}

void TexturesManager::dequeueTexture(const std::string& name) {
    auto it = textureQueue.find(name);
    if (it != textureQueue.end()) {
        textureQueue.erase(it);
        textureUnits.erase(name);
        textureCompBindingUnits.erase(name);
        nextUnit--;
        nextUnitComp--;
    }
}

void TexturesManager::dequeueAllTextures() {
    textureQueue.clear();
    textureUnits.clear();
    textureCompBindingUnits.clear();
    nextUnit = 0;
    nextUnitComp = 0;
}

void TexturesManager::reloadTextureByName(const std::string& name) {
    auto it = textureQueue.find(name);
    if (it != textureQueue.end()) {
        auto& texture = it->second;
        texture->loadTexture(texture->getPath(), texture->getType(), texture->getIsFlip(), texture->getMode(), texture->getInterpolation());
    }
}

void TexturesManager::reloadAllTextures() {
    for (auto& pair : textureQueue) {
        auto& texture = pair.second;
        texture->loadTexture(texture->getPath(), texture->getType(), texture->getIsFlip(), texture->getMode(), texture->getInterpolation());
    }
}

void TexturesManager::reloadRawTextureByName(const std::string& name) {
    auto it = textureQueue.find(name);
    if (it != textureQueue.end()) {
        auto& texture = it->second;
        texture->loadRawTexture(texture->getType(), texture->getInternalFormat(), texture->getWidth(), texture->getHeight(), texture->getIsFlip(), texture->getMode(), texture->getInterpolation());
    }
}

void TexturesManager::reloadAllRawTextures() {
    for (auto& pair : textureQueue) {
        auto& texture = pair.second;
        texture->loadRawTexture(texture->getType(), texture->getInternalFormat(), texture->getWidth(), texture->getHeight(), texture->getIsFlip(), texture->getMode(), texture->getInterpolation());
    }
}

void TexturesManager::bindAllTextures() {
    for (auto& pair : textureQueue) {
        auto& texture = pair.second;
        auto unit = textureUnits[pair.first];
        texture->bindTexture(unit, texture->getTexID());
    }
}

void TexturesManager::bindTextureByName(const std::string& name) {
    auto it = textureQueue.find(name);
    if (it != textureQueue.end()) {
        auto& texture = it->second;
        auto unit = textureUnits[name];
        texture->bindTexture(unit, texture->getTexID());
    }
}

void TexturesManager::bindAllRawTextures(int level, bool isLayered, int layer, GLenum access) {
    for (auto& pair : textureQueue) {
        auto& texture = pair.second;
        unsigned int unitComp = textureCompBindingUnits[pair.first];
        texture->bindRawTexture(unitComp, texture->getTexID(), level, isLayered, layer, access);
    }
}

void TexturesManager::bindRawTextureByName(const std::string& name, int level, bool isLayered, int layer, GLenum access) {
    auto it = textureQueue.find(name);
    if (it != textureQueue.end()) {
        auto& texture = it->second;
        unsigned int unitComp = textureCompBindingUnits[name];
        texture->bindRawTexture(unitComp, texture->getTexID(), level, isLayered, layer, access);
    }
}

void TexturesManager::unbindAllTextures() {
    for (auto& pair : textureQueue) {
        pair.second->unbindTexture();
    }
}

void TexturesManager::unbindTextureByName(const std::string& name) {
    auto it = textureQueue.find(name);
    if (it != textureQueue.end()) {
        it->second->unbindTexture();
    }
}

unsigned int TexturesManager::getTextureIDByName(const std::string& name) {
    auto it = textureQueue.find(name);
    if (it != textureQueue.end()) {
        return it->second->getTexID();
    }
    return 0;
}

unsigned int TexturesManager::getTextureUnit(const std::string& name) {
    auto it = textureUnits.find(name);
    if (it != textureUnits.end()) {
        return it->second;
    }
    return -1;
}

unsigned int TexturesManager::getTextureCompBindingUnit(const std::string& name) {
    auto it = textureCompBindingUnits.find(name);
    if (it != textureCompBindingUnits.end()) {
        return it->second;
    }
    return -1;
}

int TexturesManager::getWidthtRawTextureByName(const std::string& name) {
    auto it = textureQueue.find(name);
    if (it != textureQueue.end()) {
        auto& texture = it->second;
        return texture->getWidth();
    }
}

int TexturesManager::getHeightRawTextureByName(const std::string& name) {
    auto it = textureQueue.find(name);
    if (it != textureQueue.end()) {
        auto& texture = it->second;
        return texture->getHeight();
    }
}

int TexturesManager::getDepthRawTextureByName(const std::string& name) {
    auto it = textureQueue.find(name);
    if (it != textureQueue.end()) {
        auto& texture = it->second;
        return texture->getDepth();
    }
}

void TexturesManager::setHeightRawTextureByName(const std::string& name, int height) {
    auto it = textureQueue.find(name);
    if (it != textureQueue.end()) {
        auto& texture = it->second;
        texture->setHeight(height);
    }
}

void TexturesManager::setWidthtRawTextureByName(const std::string& name, int width) {
    auto it = textureQueue.find(name);
    if (it != textureQueue.end()) {
        auto& texture = it->second;
        texture->setWidth(width);
    }
}

void TexturesManager::setDepthRawTextureByName(const std::string& name, int depth) {
    auto it = textureQueue.find(name);
    if (it != textureQueue.end()) {
        auto& texture = it->second;
        texture->setDepth(depth);
    }
}