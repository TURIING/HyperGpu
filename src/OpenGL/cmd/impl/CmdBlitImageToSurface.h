/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/16 21:11
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef CMDBLITIMAGETOSURFACE_H
#define CMDBLITIMAGETOSURFACE_H
#include "../../../common/common.h"
#include "../CmdBase.h"
#include "../../surface/GlSurface.h"
#include "../../resource/GlImage2D.h"

USING_GPU_NAMESPACE_BEGIN

class CmdBlitImageToSurface final: public CmdBase {
public:
    CmdBlitImageToSurface(): CmdBase(CMD_TYPE) {}

    ~CmdBlitImageToSurface() override {
        Reset();
    }

    void Init(Image2D* pSrcImage, GpuSurface* pSurface, ImageBlitRange* pRange, u32 rangeCount, Filter filter) {
        LOG_ASSERT(pSrcImage && pSurface);
        m_pSurface = dynamic_cast<GlSurface*>(pSurface);
        m_pSrcImage = dynamic_cast<GlImage2D*>(pSrcImage);
        m_pSurface->AddRef();
        m_pSrcImage->AddRef();

        m_ranges.reserve(rangeCount);
        for (auto i = 0; i < rangeCount; i++) {
            m_ranges.push_back(pRange[i]);
        }
        m_filter = filter;
    }

    void Execute(GlContext* pContext) override {
        m_pSurface->Bind();
        GLuint fbo = GL_NONE;
        CALL_GL(glGenFramebuffers(1, &fbo));
        CALL_GL(glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo));
        CALL_GL(glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pSrcImage->GetHandle(), 0));
        CHECK_FRAMEBUFFER_COMPLETE(GL_READ_FRAMEBUFFER);
        CALL_GL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_pSurface->GetSurfaceFbo()));
        CHECK_FRAMEBUFFER_COMPLETE(GL_DRAW_FRAMEBUFFER);

        for (auto range: m_ranges) {
            CALL_GL(glBlitFramebuffer(
                range.srcArea.offset.x,  range.srcArea.offset.y,
                range.srcArea.offset.x + range.srcArea.size.width,
                range.srcArea.offset.y + range.srcArea.size.height,
                range.dstArea.offset.x,  range.dstArea.offset.y,
                range.dstArea.offset.x + range.dstArea.size.width,
                range.dstArea.offset.y + range.dstArea.size.height,
                GL_COLOR_BUFFER_BIT,
                gFilterToGlFilter[static_cast<int>(m_filter)]
            ));
        }
        CALL_GL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
        CALL_GL(glBindFramebuffer(GL_READ_FRAMEBUFFER, 0));
        CALL_GL(glDeleteFramebuffers(1, &fbo));
        m_pSurface->Unbind();
    }

    void Reset() override {
        m_pSrcImage->SubRef();
        m_pSrcImage = nullptr;

        m_pSurface->SubRef();
        m_pSurface = nullptr;
    }

public:
    static constexpr auto CMD_TYPE =  CmdType::BlitImageToSurface;

private:
    GlSurface* m_pSurface = nullptr;
    GlImage2D* m_pSrcImage = nullptr;
    std::vector<ImageBlitRange> m_ranges;
    Filter m_filter = {};
};

USING_GPU_NAMESPACE_END
#endif //CMDBLITIMAGETOSURFACE_H