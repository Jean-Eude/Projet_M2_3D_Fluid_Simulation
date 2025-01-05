#include <FBO.hpp>

void FBO::createFBO(int width, int height, FBOType type) {
    m_winWidth = width;
    m_winHeight = height;
    m_fboWidth = width;
    m_fboHeight = height;

	fboType = type;

    glGenFramebuffers(1, &m_fbo);
    bindFBO();

    glGenTextures(1, &texid);

    switch (type) {
        case FBOType::RENDER_TO_SCREEN:
            texType = GL_TEXTURE_2D;
            bindTexture();
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texid, 0);
            break;

        case FBOType::ENVIRONMENT_MAPPING:
            texType = GL_TEXTURE_CUBE_MAP;
            bindTexture();
            for (unsigned int i = 0; i < 6; ++i) {
    			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
            }
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texid, 0);
            break;

        case FBOType::SHADOW_MAP:
            texType = GL_TEXTURE_2D;
            bindTexture();
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            GLfloat borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texid, 0);

            // Pas de couleur pour un shadow map FBO
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
            break;
    }

    // Configuration du renderbuffer pour la profondeur et le stencil, sauf pour les shadow maps
    if (type != FBOType::SHADOW_MAP) {
        glGenRenderbuffers(1, &m_rbo);
        bindRBO();
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
    }

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";
    }

    unbindFBO();
    unbindTexture();
    unbindRBO();
}

void FBO::rescaleFBO(int width, int height) {
    m_fboWidth = width;
    m_fboHeight = height;

    bindTexture();

    if (fboType == FBOType::RENDER_TO_SCREEN) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texid, 0);
    } else if (fboType == FBOType::ENVIRONMENT_MAPPING) {
        for (unsigned int i = 0; i < 6; ++i) {
    		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
        }
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texid, 0);
    } else if (fboType == FBOType::SHADOW_MAP) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        GLfloat borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texid, 0);

        // Pas de couleur pour une shadow map
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }

    unbindTexture();

    // Mettre à jour le renderbuffer uniquement pour les types qui nécessitent profondeur et stencil
    if (fboType != FBOType::SHADOW_MAP) {
        bindRBO();
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
        unbindRBO();
    }

    glViewport(0, 0, width, height);
}


void FBO::rescaleMainWindow(int width, int height) {
    m_winWidth = width;
    m_winHeight = height;
}

void FBO::bindFBO() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

void FBO::unbindFBO() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::bindRBO() {
    glBindRenderbuffer(GL_FRAMEBUFFER, m_fbo);
}

void FBO::unbindRBO() {
    glBindRenderbuffer(GL_RENDERBUFFER, 0);    
}

void FBO::bindTexture() {
    glBindTexture(texType, texid);
}

void FBO::unbindTexture() {
    glBindTexture(texType, 0);
}

// Getters
GLuint FBO::getTexID() {
    return texid;
}

GLuint FBO::getRBO() {
    return m_rbo;
}

GLuint FBO::getFBO() {
    return m_fbo;
}

int FBO::getWinWidth() {
    return m_winWidth;
}

int FBO::getWinHeight() {
    return m_winHeight;
}

int FBO::getFBOWidth() {
    return m_fboWidth;
}

int FBO::getFBOHeight() {
    return m_fboHeight;
}