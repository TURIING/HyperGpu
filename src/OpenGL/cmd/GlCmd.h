/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/10 15:17
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef HYPERRENDER_GLCMD_H
#define HYPERRENDER_GLCMD_H

#include "CmdBase.h"
#include "../../common/common.h"

USING_GPU_NAMESPACE_BEGIN
class CmdBase;
class GlContext;
class OpenGlDevice;
class CmdBeginRenderPass;

class GlCmd final : public GpuCmd {
public:
    explicit GlCmd(OpenGlDevice* device): m_pDevice(device) {}
    ~GlCmd() override;
    void Reset() override;
    void Begin() override;
    void End() override;
    void BeginRenderPass(const BeginRenderInfo& info) override;
    void EndRenderPass() override;
    void Draw(const DrawInfo& info) override;
    void ClearColorImage(Image2D* image, const Color &color) override;
    void SetViewport(const Viewport& viewport) override;
    void SetScissor(const Scissor& scissor) override;
    void Execute(GlContext* pContext) const;
    void BlitImageToSurface(Image2D* pImage, GpuSurface* surface, ImageBlitRange* pRange, uint32_t rangeCount, Filter filter) override;
    void CopyImage(Image2D *pSrcImage, Image2D *pDstImage, ImageCopyRange *pRange, uint32_t rangeCount) override;
    void CopyBufferToImage(Image2D *pImage, const void *pData, uint64_t size, const Area &area) override;
    void CopyImageToBuffer(Image2D *pImage, Buffer* pBuffer, const Area &area) override;
    void BeginDebugUtilsLabel(const char *name, const Color &color) override;
    void EndDebugUtilsLabel() override;

private:
    template<class CmdClass, typename... Params>
    CmdClass* allocCmd(Params... params) {
        static_assert(std::is_base_of_v<CmdBase, CmdClass>);
        const CmdType type = CmdClass::CMD_TYPE;

        CmdClass* pCmd = nullptr;
        if (const auto it = m_freeCmds.find(type); it == m_freeCmds.end() || it->second.empty()) {
            pCmd = new CmdClass();
        }
        else {
            pCmd = dynamic_cast<CmdClass*>(it->second.back());
            it->second.pop_back();
        }

        assert(pCmd);
        pCmd->Init(params...);
        return pCmd;
    }

    void freeCmd(CmdBase* pCmd) {
        const auto type = pCmd->GetCmdType();
        pCmd->Reset();
        if (const auto it = m_freeCmds.find(type); it == m_freeCmds.end()) {
            m_freeCmds[type] = { pCmd };
        }
        else {
            it->second.push_back(pCmd);
        }
    }

private:
    OpenGlDevice* m_pDevice = nullptr;
    std::vector<CmdBase*> m_vecCmds;
    std::unordered_map<CmdType, std::vector<CmdBase*>> m_freeCmds;
    CmdBeginRenderPass* m_pLastCmdBeginRenderPass = nullptr;
};

USING_GPU_NAMESPACE_END
#endif //HYPERRENDER_GLCMD_H