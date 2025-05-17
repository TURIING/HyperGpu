/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/10 15:17
* @version: 1.0
* @description: 
********************************************************************************/
#include "GlCmd.h"

#include "CmdBase.h"
#include "impl/CmdBlitImageToSurface.h"
#include "impl/CmdClearColorImage.h"

GlCmd::~GlCmd() {
    for (auto pCmd: m_vecCmds) {
        pCmd->SubRef();
    }

    for (auto &pair: m_freeCmds) {
        for (const auto pCmd: pair.second) {
            pCmd->SubRef();
        }
    }
}

void GlCmd::Reset() {
    for (auto pCmd: m_vecCmds) {
        freeCmd(pCmd);
    }
    m_vecCmds.clear();
}

void GlCmd::Begin() {
}

void GlCmd::End() {
}

void GlCmd::BeginRenderPass(const BeginRenderInfo& info) {
}

void GlCmd::EndRenderPass() {
}

void GlCmd::Draw(const DrawInfo& info) {
}

void GlCmd::ClearColorImage(Image2D* image, const Color &color) {
    const auto cmd = allocCmd<CmdClearColorImage>(image, color);
    m_vecCmds.push_back(cmd);
}

void GlCmd::SetViewport(const Viewport& viewport) {
}

void GlCmd::SetScissor(const Scissor& scissor) {
}

void GlCmd::Execute(GlContext* pContext) {
    for (auto pCmd : m_vecCmds) {
        pCmd->Execute(pContext);
    }
}

void GlCmd::BlitImageToSurface(Image2D* pImage, GpuSurface* surface, ImageBlitRange* pRange, uint32_t rangeCount, Filter filter) {
    const auto pCmd = this->allocCmd<CmdBlitImageToSurface>(pImage, surface, pRange, rangeCount, filter);
    m_vecCmds.push_back(pCmd);
}
