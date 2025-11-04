//
// Created by turiing on 2025/11/1.
//

#ifndef CMDCOPYIMAGETOBUFFER_H
#define CMDCOPYIMAGETOBUFFER_H

#include "../../../common/common.h"
#include "../CmdBase.h"
#include "../../resource/GlImage2D.h"
#include "../../resource/GlBuffer.h"

USING_GPU_NAMESPACE_BEGIN

class CmdCopyImageToBuffer final: public CmdBase {
public:
    static constexpr auto CMD_TYPE =  CmdType::CopyImageToBuffer;
    CmdCopyImageToBuffer(): CmdBase(CMD_TYPE) {}
    ~CmdCopyImageToBuffer() override { Reset(); }

    void Init(Image2D *pImage, Buffer* pBuffer, const Area &area) {
        m_pSrcImage = dynamic_cast<GlImage2D*>(pImage);
        m_pDstBuffer = dynamic_cast<GlBuffer*>(pBuffer);
        m_area = area;
    }

    void Execute(GlContext* pContext) override {
// #if defined(PLATFORM_MACOS) || defined(PLATFORM_IOS)
//         auto srcX0 = m_pRange->srcArea.offset.x;
//         auto srcY0 = m_pRange->srcArea.size.height - m_pRange->srcArea.offset.y;
//         auto srcX1 = m_pRange->srcArea.offset.x + m_pRange->srcArea.size.width;
//         auto srcY1 = m_pRange->srcArea.size.height - m_pRange->srcArea.offset.y;
//
//         auto dstX0 = m_pRange->dstArea.offset.x;
//         auto dstY0 = m_pRange->dstArea.size.height - m_pRange->dstArea.offset.y;
//         auto dstX1 = m_pRange->dstArea.offset.x + m_pRange->dstArea.size.width;
//         auto dstY1 = m_pRange->dstArea.size.height - m_pRange->dstArea.offset.y;
//
//         auto aspectSrc = gImageAspectToGlAttachmentType[TO_I32(m_pSrcImage->GetAspectFlags())];
//         auto aspectDst = gImageAspectToGlAttachmentType[TO_I32(m_pDstImage->GetAspectFlags())];
//         LOG_ASSERT(aspectSrc == aspectDst);
//
//         GLuint fbos[2];
//         CALL_GL(glGenFramebuffers(2, fbos));
//         CALL_GL(glBindFramebuffer(GL_READ_FRAMEBUFFER, fbos[0]));
//         CALL_GL(glFramebufferTexture2D(GL_READ_FRAMEBUFFER, aspectSrc, GL_TEXTURE_2D, m_pSrcImage->GetHandle(), 0));
//
//         CALL_GL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbos[1]));
//         CALL_GL(glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, aspectDst, GL_TEXTURE_2D, m_pDstImage->GetHandle(), 0));
//
//         CALL_GL(glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, aspectSrc, GL_LINEAR));
//
//         // todo 使用glInvalidateSubFramebuffer加快性能
// #endif
    }

    void Reset() override {}

private:
    GlImage2D *m_pSrcImage = nullptr;
    GlBuffer* m_pDstBuffer = nullptr;
    Area m_area;
};

USING_GPU_NAMESPACE_END
#endif //CMDCOPYIMAGETOBUFFER_H
