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
#include "../../pipeline/GlGraphicPipeline.h"

USING_GPU_NAMESPACE_BEGIN

class CmdDraw final: public CmdBase {
public:
    static constexpr auto CMD_TYPE =  CmdType::Draw;
    CmdDraw(): CmdBase(CMD_TYPE) {}
    ~CmdDraw() override { Reset(); }

    void Init(const DrawInfo &drawInfo, const CmdBeginRenderPass* pCmdBeginPass) {
        LOG_ASSERT(pCmdBeginPass);
        m_drawInfo = drawInfo;
        m_pCmdBeginRenderPass = pCmdBeginPass;
        m_pPipeline = dynamic_cast<GlGraphicPipeline*>(m_pCmdBeginRenderPass->GetBeginRenderInfo().pPipeline);
        m_pPipeline->SetUniforms(drawInfo.pUniformBinding, drawInfo.uniformBindingCount);
        m_pPipeline->SetTextures(drawInfo.pImageBinding, drawInfo.imageBindingCount);
    }

    void Execute(GlContext* pContext) override {
        const BeginRenderInfo& beginInfo = m_pCmdBeginRenderPass->GetBeginRenderInfo();
        auto pInputAssembly = dynamic_cast<GlInputAssembler*>(m_drawInfo.pInputAssembler);

        // Blend
        {
            const auto &blendInfo = m_pPipeline->GetBlendInfo();
            if (blendInfo.enable) {
                auto srcColorFactor = gBlendFactorToGlBlendFactor[TO_I32(blendInfo.srcColorBlendFactor)];
                auto dstColorFactor = gBlendFactorToGlBlendFactor[TO_I32(blendInfo.dstColorBlendFactor)];
                auto srcAlphaFactor = gBlendFactorToGlBlendFactor[TO_I32(blendInfo.srcAlphaBlendFactor)];
                auto dstAlphaFactor = gBlendFactorToGlBlendFactor[TO_I32(blendInfo.dstAlphaBlendFactor)];
                auto colorBlendOp = gBlendOpToGlBlendOp[TO_I32(blendInfo.colorBlendOp)];
                auto alphaBlendOp = gBlendOpToGlBlendOp[TO_I32(blendInfo.alphaBlendOp)];

                CALL_GL(glEnable(GL_BLEND));
                CALL_GL(glBlendFuncSeparate(srcColorFactor, dstColorFactor, srcAlphaFactor, dstAlphaFactor));
                CALL_GL(glBlendEquationSeparate(colorBlendOp, alphaBlendOp));
                CALL_GL(glBlendColor(blendInfo.constantColor.r, blendInfo.constantColor.g, blendInfo.constantColor.b, blendInfo.constantColor.a));
                if (blendInfo.colorWriteMask != (ColorComponentFlags::R | ColorComponentFlags::G | ColorComponentFlags::B | ColorComponentFlags::A)) {
                    const auto red = has_flag(blendInfo.colorWriteMask, ColorComponentFlags::R);
                    const auto green = has_flag(blendInfo.colorWriteMask, ColorComponentFlags::G);
                    const auto blue = has_flag(blendInfo.colorWriteMask, ColorComponentFlags::B);
                    const auto alpha = has_flag(blendInfo.colorWriteMask, ColorComponentFlags::A);
                    CALL_GL(glColorMask(red, green, blue, alpha));
                }
            }
            else {
                CALL_GL(glDisable(GL_BLEND));
            }
        }

        auto isLastScissorEnabled = glIsEnabled(GL_SCISSOR_TEST);
        GLint lastScissorValue[4];
        if (isLastScissorEnabled) {
            CALL_GL(glGetIntegerv(GL_SCISSOR_BOX, lastScissorValue));
        }

        if (beginInfo.renderAttachmentType == RenderAttachmentType::Image2D) {
            std::vector<GLuint> vecAttachments;
            const auto imageCount = beginInfo.renderAttachment.imageCount;

            GLuint fbo;
            CALL_GL(glGenFramebuffers(1, &fbo));
            CALL_GL(glBindFramebuffer(GL_FRAMEBUFFER, fbo));

            for (int i = 0; i < imageCount; i++) {
                auto pImage = dynamic_cast<GlImage2D*>(beginInfo.renderAttachment.image[i]);
                const auto attachment = gImageAspectToGlAttachmentType[static_cast<int>(pImage->GetAspectFlags())];
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
                switch (pImage->GetAspectFlags()) {
                    case ImageAspectFlags::Color: {
                        LOG_ASSERT(beginInfo.clearValue[i].type == AttachmentType::COLOR);
                        const auto color = beginInfo.clearValue[i].color;
                        CALL_GL(glClearColor(color.r, color.g, color.b, color.a));
                        CALL_GL(glClear(GL_COLOR_BUFFER_BIT));
                        break;
                    }
                    case ImageAspectFlags::Depth_Stencil: {
                        LOG_ASSERT(beginInfo.clearValue[i].type == AttachmentType::DEPTH_STENCIL);
                        const auto depth = beginInfo.clearValue[i].depthStencil.depth;
                        const auto stencil = beginInfo.clearValue[i].depthStencil.stencil;
                        CALL_GL(glClearDepth(depth))
                        CALL_GL(glClearStencil(stencil));
                        CALL_GL(glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
                        break;
                    }
                    default:    LOG_ASSERT(false);
                }
            }

            if (isLastScissorEnabled) {
                CALL_GL(glEnable(GL_SCISSOR_TEST));
                CALL_GL(glScissor(lastScissorValue[0], lastScissorValue[1], lastScissorValue[2], lastScissorValue[3]));
            }
            else {
                CALL_GL(glDisable(GL_SCISSOR_TEST));
            }

            draw(pInputAssembly, dynamic_cast<GlGraphicPipeline *>(beginInfo.pPipeline));
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

            if (isLastScissorEnabled) {
                CALL_GL(glEnable(GL_SCISSOR_TEST));
                CALL_GL(glScissor(lastScissorValue[0], lastScissorValue[1], lastScissorValue[2], lastScissorValue[3]));
            }
            else {
                CALL_GL(glDisable(GL_SCISSOR_TEST));
            }

            draw(pInputAssembly, dynamic_cast<GlGraphicPipeline *>(beginInfo.pPipeline));

            pSurface->Unbind();
        }
    }

    void Reset() override {
    }

private:
    static void draw(const GlInputAssembler* pInputAssembly, const GlGraphicPipeline* pPipeline) {
        pInputAssembly->Bind();
        pPipeline->Bind();
        if (auto instanceCount = pInputAssembly->GetInstanceCount(); instanceCount == 0) {
            if (auto indexCount = pInputAssembly->GetIndexCount(); indexCount > 0) {
                CALL_GL(glDrawElements(gPrimitiveTypeToGlType[TO_I32(pPipeline->GetPrimitiveType())], indexCount, GL_UNSIGNED_INT, 0));
            }
            else {
                CALL_GL(glDrawArrays(gPrimitiveTypeToGlType[TO_I32(pPipeline->GetPrimitiveType())], 0, pInputAssembly->GetVertexCount()));
            }
        }
        else {
            if (auto indexCount = pInputAssembly->GetIndexCount(); indexCount > 0) {
                CALL_GL(glDrawElementsInstanced(gPrimitiveTypeToGlType[TO_I32(pPipeline->GetPrimitiveType())], indexCount, GL_UNSIGNED_INT, 0, instanceCount));
            }
            else {
                CALL_GL(glDrawArraysInstanced(gPrimitiveTypeToGlType[TO_I32(pPipeline->GetPrimitiveType())], 0, pInputAssembly->GetVertexCount(), instanceCount));
            }
        }
    }

private:
    const CmdBeginRenderPass* m_pCmdBeginRenderPass = nullptr;
    GlGraphicPipeline* m_pPipeline = nullptr;
    DrawInfo m_drawInfo {};
};

USING_GPU_NAMESPACE_END
#endif //CMDDRAW_H
