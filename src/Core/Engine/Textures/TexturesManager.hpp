#pragma once

#include <Texture.hpp>
#include <ThreadManager.hpp>

class TexturesManager {
    private:
        std::map<std::string, std::shared_ptr<Texture>> textureQueue;
        std::map<std::string, unsigned int> textureUnits;
        std::map<std::string, unsigned int> textureCompBindingUnits;

        unsigned int nextUnit;
        unsigned int nextUnitComp;

    public:
        TexturesManager() : nextUnit(0), nextUnitComp(0) {}
        ~TexturesManager() = default;

        // Textures
        void enqueueTexture(const std::string& name, const std::string& TexPath, TexType type, bool isFlip, GLint mode, GLint interpolation);
        void reloadTextureByName(const std::string& name);
        void reloadAllTextures();

        // Textures vides
        void enqueueRawTexture(const std::string& name, TexType type, TexInternalFormat texInternal, int width, int height, bool isFlip, GLint mode, GLint interpolation);
        void reloadRawTextureByName(const std::string& name);
        void reloadAllRawTextures();

        int getWidthtRawTextureByName(const std::string& name);
        int getHeightRawTextureByName(const std::string& name);
        int getDepthRawTextureByName(const std::string& name);
        void setWidthtRawTextureByName(const std::string& name, int width);
        void setHeightRawTextureByName(const std::string& name, int height);
        void setDepthRawTextureByName(const std::string& name, int depth);

        // Général
        void dequeueTexture(const std::string& name);
        void dequeueAllTextures();
        void bindAllTextures();
        void bindTextureByName(const std::string& name);
        void bindAllRawTextures(int level, bool isLayered, int layer, GLenum access);
        void bindRawTextureByName(const std::string& name, int level, bool isLayered, int layer, GLenum access);
        void unbindAllTextures();
        void unbindTextureByName(const std::string& name);

        unsigned int getTextureIDByName(const std::string& name);
        unsigned int getTextureUnit(const std::string& name); 
        unsigned int getTextureCompBindingUnit(const std::string& name); 
};