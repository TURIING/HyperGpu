/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/17 15:20
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef OPENGLSYNCMANAGER_H
#define OPENGLSYNCMANAGER_H

#include "../../common/common.h"

USING_GPU_NAMESPACE_BEGIN

class OpenGlDevice;
class OpenGlSyncManager final: public GpuSyncManager {
public:
    explicit OpenGlSyncManager(OpenGlDevice* pDevice);
    ~OpenGlSyncManager() override;
    [[nodiscard]] Fence* CreateFence() override;
    [[nodiscard]] Semaphore* CreateSemaphore() override;

private:
    OpenGlDevice* m_pDevice = nullptr;
};


USING_GPU_NAMESPACE_END
#endif //OPENGLSYNCMANAGER_H
