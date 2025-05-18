/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/18 13:40
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef AGLSURFACE_H
#define AGLSURFACE_H

#include "../../common/common.h"
#include "GlSurface.h"
#include "../core/OpenGlDevice.h"
#include "../context/GlContext.h"
USING_GPU_NAMESPACE_BEGIN

class AGlSurface final: public GlSurface {
public:
    AGlSurface(OpenGlDevice* pDevice, const PlatformWindowInfo &info): GlSurface(pDevice) {
        m_pContext->MakeCurrent();
#if PLATFORM_IOS
        CALL_GL(glGenRenderbuffers(1, &m_rbo));
        CALL_GL(glBindRenderbuffer(GL_RENDERBUFFER, m_rbo));
#endif
        m_pContext->BindLayer(info.handle);
#if PLATFORM_IOS
        CALL_GL(glGenFramebuffers(1, &m_fbo));
        CALL_GL(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
        CALL_GL(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_rbo));
        CHECK_FRAMEBUFFER_COMPLETE(GL_FRAMEBUFFER);
#endif
    }

    void SwapBuffers() override {
        m_pContext->SwapBuffer();
    }

private:
#if PLATFORM_IOS
    GLuint m_rbo = GL_NONE;
    GLuint m_fbo = GL_NONE;
#endif
};
USING_GPU_NAMESPACE_END
#endif //AGLSURFACE_H
