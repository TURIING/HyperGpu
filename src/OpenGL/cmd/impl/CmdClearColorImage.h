/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/17 14:24
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef CMDCLEARCOLORIMAGE_H
#define CMDCLEARCOLORIMAGE_H

#include "../../../common/common.h"
#include "../CmdBase.h"
#include "../../resource/GlImage2D.h"

USING_GPU_NAMESPACE_BEGIN

class CmdClearColorImage final: public CmdBase {
public:
    static constexpr auto CMD_TYPE =  CmdType::ClearColorImage;
    CmdClearColorImage(): CmdBase(CMD_TYPE) {}
    ~CmdClearColorImage() override { Reset(); }

    void Init(Image2D* pImage, const Color &color) {
        LOG_ASSERT(pImage);
        m_pImage = dynamic_cast<GlImage2D*>(pImage);
        m_pImage->AddRef();
        m_color = color;
    }

    void Execute(GlContext* pContext) override {
        GLuint fbo = GL_NONE;
        CALL_GL(glGenFramebuffers(1, &fbo));
        CALL_GL(glBindFramebuffer(GL_FRAMEBUFFER, fbo));
        CALL_GL(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pImage->GetHandle(), 0));
        CALL_GL(glClearColor(m_color.r, m_color.g, m_color.b, m_color.a));
        CALL_GL(glClear(GL_COLOR_BUFFER_BIT));
        CALL_GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        CALL_GL(glDeleteFramebuffers(1, &fbo));
    }

    void Reset() override {
        m_pImage->SubRef();
    }

private:
    GlImage2D* m_pImage = nullptr;
    Color m_color;
};

USING_GPU_NAMESPACE_END
#endif //CMDCLEARCOLORIMAGE_H