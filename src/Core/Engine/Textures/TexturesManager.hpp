#pragma once

#include <Texture.hpp>
#include <ThreadManager.hpp>

class TexturesManager {
    private:
        std::map<std::shared_ptr<Texture>, unsigned int> textureQueue;
        unsigned int nextUnit = 0;
        unsigned int nextUnitComp = 0;

    public:
        TexturesManager() = default;
        ~TexturesManager() = default;

        // Textures
        void enqueueTexture(const std::string& name, const std::string& TexPath, TexType type, bool isFlip, GLint mode, GLint interpolation);
        void reloadTextureByName(const std::string& name);
        void reloadAllTextures();

        // Textures vides
        void enqueueRawTexture(const std::string& name, TexType type, TexInternalFormat texInternal, bool isCompute, int width, int height, bool isFlip, GLint mode, GLint interpolation);
        void reloadRawTextureByName(const std::string& name);
        void reloadAllRawTextures();

        // Général
        void dequeueTexture(const std::string& name);
        void dequeueAllTextures();
        void bindAllTextures();
        void bindTextureByName(const std::string& name);
        void unbindAllTextures();
        void unbindTextureByName(const std::string& name);

        unsigned int getTextureIDByName(const std::string& name);
        unsigned int getTextureUnit(const std::string& name); 
};