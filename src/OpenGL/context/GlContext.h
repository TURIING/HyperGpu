/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/9 21:57
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef HYPERRENDER_GLCONTEXT_H
#define HYPERRENDER_GLCONTEXT_H

#include "../../common/common.h"

USING_GPU_NAMESPACE_BEGIN

class IContext;
class OpenGlDevice;

class GlContext final : public GpuObject {
public:
    GlContext(OpenGlDevice* pGlDevice, GlContext* shareContext);
    ~GlContext() override;
    void MakeCurrent() const;
    void ClearCurrent() const;
    void PushAndMakeCurrent();
    void PopAndMakeCurrent();
    void BindLayer(void *layer);
    void SwapBuffer();
    static void GlSyncFinish();
    static bool IsInContext();

private:
    void init();

private:
    OpenGlDevice* m_pGlDevice = nullptr;
    GlContext* m_pShareContext = nullptr;
    IContext* m_pBaseContext = nullptr;
    IContext* m_pPreContext = nullptr;
    std::mutex m_lockContext;
};

USING_GPU_NAMESPACE_END
#endif //HYPERRENDER_GLCONTEXT_H
