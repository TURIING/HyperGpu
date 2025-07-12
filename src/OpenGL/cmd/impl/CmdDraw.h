//
// Created by turiing on 2025/7/5.
//

#ifndef CMDDRAW_H
#define CMDDRAW_H

#include "../../../common/common.h"
#include "../CmdBase.h"
#include "CmdBeginRenderPass.h"
#include "../../resource/GlInputAssembler.h"
#include "../../resource/GlImage2D.h"
#include "../../surface/GlSurface.h"
#include "../../pipeline/GlPipeline.h"

USING_GPU_NAMESPACE_BEGIN

constexpr GLenum gImageUsageToGlAttachmentType[] = {
    GL_COLOR_ATTACHMENT0,           // Color
    GL_DEPTH_ATTACHMENT,            // Depth
    GL_STENCIL_ATTACHMENT,          // Stencil
};

class CmdDraw final: public CmdBase {
public:
    static constexpr auto CMD_TYPE =  CmdType::Draw;
    CmdDraw(): CmdBase(CMD_TYPE) {}
    ~CmdDraw() override { Reset(); }

    void Init(const DrawInfo &drawInfo, const CmdBeginRenderPass* pCmdBeginPass) {
        LOG_ASSERT(pCmdBeginPass);
        m_drawInfo = &drawInfo;
        m_pCmdBeginRenderPass = pCmdBeginPass;
    }

    void Execute(GlContext* pContext) override {
        const BeginRenderInfo& beginInfo = m_pCmdBeginRenderPass->GetBeginRenderInfo();
        auto pInputAssembly = dynamic_cast<GlInputAssembler*>(m_drawInfo->pInputAssembler);

        auto isLastScissorEnabled = glIsEnabled(GL_SCISSOR_TEST);
        GLint lastScissorValue[4];
        CALL_GL(glGetIntegerv(GL_SCISSOR_BOX, lastScissorValue));

        if (beginInfo.renderAttachmentType == RenderAttachmentType::Image2D) {
            std::vector<GLuint> vecAttachments;
            const auto imageCount = beginInfo.renderAttachment.imageCount;

            GLuint fbo;
            CALL_GL(glGenFramebuffers(1, &fbo));
            CALL_GL(glBindFramebuffer(GL_FRAMEBUFFER, fbo));

            for (int i = 0; i < imageCount; i++) {
                auto pImage = dynamic_cast<GlImage2D*>(beginInfo.renderAttachment.image[i]);
                const auto attachment = gImageUsageToGlAttachmentType[static_cast<int>(pImage->GetUsage())];
                vecAttachments.push_back(attachment);

                CALL_GL(glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, pImage->GetHandle(), 0));

                // clear color
                if (beginInfo.clearValue.empty()) continue;
                LOG_ASSERT_INFO(
                    beginInfo.clearValue.size() == beginInfo.renderAttachment.imageCount,
                    "Number of BeginRenderInfo::clearValue needs to be equal to number of BeginRenderInfo ::renderAttachment::imageCount."
                );
                const auto imageHeight = pImage->GetSize().height;
                const auto &renderArea = beginInfo.renderArea;
                CALL_GL(glScissor(renderArea.offset.x, imageHeight - renderArea.offset.y, renderArea.size.width, renderArea.size.height));
                CALL_GL(glEnable(GL_SCISSOR_TEST));
                switch (pImage->GetUsage()) {
                    case Image2D::ImageUsage::Color: {
                        LOG_ASSERT(beginInfo.clearValue[i].type == AttachmentType::COLOR);
                        const auto color = beginInfo.clearValue[i].color;
                        CALL_GL(glClearColor(color.r, color.g, color.b, color.a));
                        CALL_GL(glClear(GL_COLOR_BUFFER_BIT));
                    }
                    case Image2D::ImageUsage::Depth_Stencil: {
                        LOG_ASSERT(beginInfo.clearValue[i].type == AttachmentType::DEPTH_STENCIL);
                        const auto depth = beginInfo.clearValue[i].depthStencil.depth;
                        const auto stencil = beginInfo.clearValue[i].depthStencil.stencil;
                        CALL_GL(glClearDepth(depth))
                        CALL_GL(glClearStencil(stencil));
                        CALL_GL(glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
                    }
                    default:    LOG_ASSERT(false);
                }
            }
            draw(pInputAssembly, dynamic_cast<GlPipeline *>(beginInfo.pPipeline));
            CALL_GL(glDeleteFramebuffers(1, &fbo));
        }
        else {
            auto pSurface = dynamic_cast<GlSurface*>(beginInfo.surface);
            pSurface->Bind();
            CALL_GL(glBindFramebuffer(GL_FRAMEBUFFER, pSurface->GetSurfaceFbo()));

            if (!beginInfo.clearValue.empty()) {
                const auto imageHeight = pSurface->GetSize().height;
                const auto &renderArea = beginInfo.renderArea;
                CALL_GL(glScissor(renderArea.offset.x, imageHeight - renderArea.offset.y, renderArea.size.width, renderArea.size.height));
                CALL_GL(glEnable(GL_SCISSOR_TEST));

                LOG_ASSERT(beginInfo.clearValue[0].type == AttachmentType::COLOR);
                const auto color = beginInfo.clearValue[0].color;
                CALL_GL(glClearColor(color.r, color.g, color.b, color.a));
                CALL_GL(glClear(GL_COLOR_BUFFER_BIT));
            }
            draw(pInputAssembly, dynamic_cast<GlPipeline *>(beginInfo.pPipeline));

            CALL_GL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
            pSurface->Unbind();
        }

        if (isLastScissorEnabled) {
            CALL_GL(glEnable(GL_SCISSOR_TEST));
            CALL_GL(glScissor(lastScissorValue[0], lastScissorValue[1], lastScissorValue[2], lastScissorValue[3]));
        }
    }

    void Reset() override {
    }

private:
    void draw(GlInputAssembler* pInputAssembly, GlPipeline* pPipeline) {
        pInputAssembly->Bind();
        pPipeline->Bind();
        if (pInputAssembly->GetIndexCount() > 0) {
            CALL_GL(glDrawElements(gPrimitiveTypeToGlType[static_cast<int>(pPipeline->GetPrimitiveType())], pInputAssembly->GetVertexCount(), GL_UNSIGNED_INT, 0));
        }
        else {
            CALL_GL(glDrawArrays(gPrimitiveTypeToGlType[static_cast<int>(pPipeline->GetPrimitiveType())], 0, pInputAssembly->GetVertexCount()));
        }
        pInputAssembly->UnBind();
    }

private:
    const CmdBeginRenderPass* m_pCmdBeginRenderPass = nullptr;
    const DrawInfo *m_drawInfo = nullptr;
};

USING_GPU_NAMESPACE_END
#endif //CMDDRAW_H
