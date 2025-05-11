/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/11 14:22
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef OPENGLCMDMANAGER_H
#define OPENGLCMDMANAGER_H

#include "../../common/common.h"

USING_GPU_NAMESPACE_BEGIN

class OpenGlDevice;

class OpenGlCmdManager final: public GpuCmdManager {
public:
    explicit OpenGlCmdManager(OpenGlDevice* pDevice): m_pDevice(pDevice) {}
    [[nodiscard]] GpuCmd* CreateCommandBuffer() override;
    void WithSingleCmdBuffer(const std::function<void(GpuCmd*)>& func) override;
    ~OpenGlCmdManager() override;

private:
    OpenGlDevice* m_pDevice = nullptr;
};

USING_GPU_NAMESPACE_END
#endif //OPENGLCMDMANAGER_H
