/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/10 15:17
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef HYPERRENDER_GLCMD_H
#define HYPERRENDER_GLCMD_H

#include "../../common/common.h"

USING_GPU_NAMESPACE_BEGIN
class CmdBase;
class GlContext;

class GlCmd final : public GpuCmd {
public:
    ~GlCmd() override;
    void Reset() override;
    void Begin() override;
    void End() override;
    void BeginRenderPass(const BeginRenderInfo& info) override;
    void EndRenderPass() override;
    void Draw(const DrawInfo& info) override;
    void ClearColorImage(Image2D* image, Color color) override;
    void SetViewport(const Viewport& viewport) override;
    void SetScissor(const Scissor& scissor) override;
    void BlitImageToSurface(Image2D* pImage, GpuSurface* surface, const ImageBlitRange& range, Filter filter) override;
    void Execute(GlContext* pContext);

private:
    std::vector<CmdBase*> m_vecCmds;
};

USING_GPU_NAMESPACE_END
#endif //HYPERRENDER_GLCMD_H