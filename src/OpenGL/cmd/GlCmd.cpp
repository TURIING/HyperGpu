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
#include "impl/CmdSetViewport.h"
#include "impl/CmdSetScissor.h"
#include "impl/CmdDraw.h"
#include "impl/CmdBeginRenderPass.h"

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
    m_pLastCmdBeginRenderPass = nullptr;
}

void GlCmd::Begin() {
}

void GlCmd::End() {
}

void GlCmd::BeginRenderPass(const BeginRenderInfo& info) {
    const auto cmd = allocCmd<CmdBeginRenderPass>(info);
    m_vecCmds.push_back(cmd);
    m_pLastCmdBeginRenderPass = cmd;
}

void GlCmd::EndRenderPass() {
    m_pLastCmdBeginRenderPass = nullptr;
}

void GlCmd::Draw(const DrawInfo& info) {
    const auto cmd = allocCmd<CmdDraw>(info, m_pLastCmdBeginRenderPass);
    m_vecCmds.push_back(cmd);
}

void GlCmd::ClearColorImage(Image2D* image, const Color &color) {
    const auto cmd = allocCmd<CmdClearColorImage>(image, color);
    m_vecCmds.push_back(cmd);
}

void GlCmd::SetViewport(const Viewport& viewport) {
    const auto cmd = allocCmd<CmdSetViewport>(viewport);
    m_vecCmds.push_back(cmd);
}

void GlCmd::SetScissor(const Scissor& scissor) {
    const auto cmd = allocCmd<CmdSetScissor>(scissor);
    m_vecCmds.push_back(cmd);
}

void GlCmd::Execute(GlContext* pContext) const {
    for (const auto pCmd : m_vecCmds) {
        pCmd->Execute(pContext);
    }
}

void GlCmd::BlitImageToSurface(Image2D* pImage, GpuSurface* surface, ImageBlitRange* pRange, uint32_t rangeCount, Filter filter) {
    const auto pCmd = this->allocCmd<CmdBlitImageToSurface>(pImage, surface, pRange, rangeCount, filter);
    m_vecCmds.push_back(pCmd);
}

void GlCmd::CopyImage(Image2D *pSrcImage, Image2D *pDstImage, ImageCopyRange *pRange, uint32_t rangeCount) {
}

void GlCmd::CopyBufferToImage(Image2D *pImage, const void *pData, uint64_t size, const Area &area) {
}

void GlCmd::BeginDebugUtilsLabel(const char *name, const Color &color) {
}

void GlCmd::EndDebugUtilsLabel() {
}
