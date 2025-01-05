#pragma once

#include <HeadersBase.hpp>

enum class FBOType {
    RENDER_TO_SCREEN,       // FBO classique pour le rendu (e.g., ImGui)
    ENVIRONMENT_MAPPING,    // FBO pour les cube maps (environnement)
    SHADOW_MAP              // FBO pour les shadow maps (par exemple, depth-only)
};

class FBO {
    public :
        FBO() = default;
        ~FBO() = default;
        
        void createFBO(int width, int height, FBOType type);
        void rescaleMainWindow(int width, int height);
        void rescaleFBO(int width, int height);

        void bindFBO();
        void unbindFBO();
        void bindRBO();
        void unbindRBO();
        void bindTexture();
        void unbindTexture();

        GLuint getTexID();
        GLuint getRBO();
        GLuint getFBO();
        int getWinWidth();
        int getWinHeight();
        int getFBOWidth();
        int getFBOHeight();
        GLuint texid;

    private :
        GLuint m_fbo;
        GLuint m_rbo;

        int m_fboWidth, m_fboHeight;
        int m_winWidth, m_winHeight;

        GLenum texType;

        FBOType fboType;
};