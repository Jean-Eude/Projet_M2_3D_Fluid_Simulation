#pragma once

#include <HeadersBase.hpp>

enum TexType {
    TEX_1D,
    TEX_2D, 
    TEX_3D,
    TEX_COMPUTE_SHADER,
};

enum TexInternalFormat {
    TEX_R32F,
    TEX_RGB32F,
    TEX_RGBA32F,
    TEX_R32UI,
    TEX_RGB32UI,
    TEX_RGBA32UI,
};

class Texture {
    public:
        Texture() : texID(0) {}
        ~Texture();
    
        void loadTexture(const std::string& TexPath, TexType type, bool isFlip, GLint mode, GLint interpolation);
        void loadRawTexture(TexType type, TexInternalFormat texInternal, bool isCompute, GLuint unit, int width, int height, bool isFlip, GLint mode, GLint interpolation);

        void unbindTexture();
        void bindTexture(unsigned int unitTex, GLuint texID);

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
        bool getIsCompute();

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

        int width, height;
        bool isCompute;

        std::string m_name;
};