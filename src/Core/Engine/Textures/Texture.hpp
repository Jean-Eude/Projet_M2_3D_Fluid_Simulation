#pragma once

#include <HeadersBase.hpp>

enum TexType {
    TEX_1D,
    TEX_2D, 
    TEX_3D,
    TEX_CUBEMAP,
    TEX_COMPUTE_SHADER,
};

enum TexInternalFormat {
    TEX_R16F,
    TEX_RGB16F,
    TEX_RGBA16F,
    TEX_R32F,
    TEX_RGB32F,
    TEX_RGBA32F,
    TEX_R32UI,
    TEX_RGB32UI,
    TEX_RGBA32UI,
};

class Texture {
    public:
        Texture() : texID(0), width(512), height(512), depth(1) {}
        ~Texture();
    
        void loadTexture(const std::string& TexPath, TexType type, bool isFlip, GLint mode, GLint interpolation);
        void loadRawTexture(TexType type, TexInternalFormat texInternal, int width, int height, bool isFlip, GLint mode, GLint interpolation);

        void unbindTexture();
        void bindTexture(unsigned int unitTex, GLuint texID);
        void bindRawTexture(unsigned int unitBinding, unsigned int texID, int level, bool isLayered, int layer, GLenum access);

        unsigned int getTexID();
        std::string getPath();
        TexType getType();
        bool getIsFlip();
        GLenum getTarget();
        GLint getMode();
        GLint getInterpolation();

        std::string& getName();
        void setName(const std::string& name);

        int getWidth();
        int getHeight();
        int getDepth();
        void setWidth(int width);
        void setHeight(int height);
        void setDepth(int depth);

        TexInternalFormat getInternalFormat();
        
    private:
        GLuint texID;
        std::string texPath;
        TexType type;
        TexInternalFormat internal;

        bool isFlip;
        GLenum target;
        GLint mode;
        GLint interpolation;

        int level; 
        bool isLayered;
        int layer;
        GLenum access;
        GLenum format;

        int width, height, depth;

        std::string m_name;
};