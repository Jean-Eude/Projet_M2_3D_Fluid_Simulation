#pragma once

#include <HeadersBase.hpp>

class FBO {
    public :
        FBO() = default;
        ~FBO() = default;
        
        void createFBO(int width, int height);
        void bindFBO();
        void unbindFBO();
        void rescaleMainWindow(int width, int height);
        void rescaleFBO(int width, int height);

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
};