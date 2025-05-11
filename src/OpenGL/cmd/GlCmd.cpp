/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/10 15:17
* @version: 1.0
* @description: 
********************************************************************************/
#include "GlCmd.h"

#include "CmdBase.h"

GlCmd::~GlCmd() {
}

void GlCmd::Reset() {
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

void GlCmd::ClearColorImage(Image2D* image, Color color) {
}

void GlCmd::SetViewport(const Viewport& viewport) {
}

void GlCmd::SetScissor(const Scissor& scissor) {
}

void GlCmd::BlitImageToSurface(Image2D* pImage, GpuSurface* surface, const ImageBlitRange& range, Filter filter) {
}

void GlCmd::Execute(GlContext* pContext) {
    for (auto pCmd : m_vecCmds) {
        pCmd->Execute(pContext);
    }
}
