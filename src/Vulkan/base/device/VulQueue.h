/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/2/24 22:58
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULQUEUE_H
#define VULQUEUE_H

#include <GpuDevice.h>

#include "../../../common/common.h"
#include "../VulObject.h"

USING_GPU_NAMESPACE_BEGIN

class VulSwapChain;
class VulCommandBuffer;
class VulFence;
class VulSemaphore;
class VulLogicDevice;

class VulQueue final: public VulObject<VkQueue> {
public:
    struct VulQueueSubmitInfo {
        VkSemaphore* pWaitSemaphores = nullptr;
        u32 waitSemaphoreCount = 0;
        VkSemaphore* pSignalSemaphores = nullptr;
        u32 signalSemaphoreCount = 0;
        VkFence pFence = nullptr;
        VkCommandBuffer* pCmd = nullptr;
        u32 cmdCount = 0;
        VkPipelineStageFlags waitDstStageMask {};
    };

    struct VulQueuePresentInfo {
        VulSwapChain* pSwapChain = nullptr;
        u32 imageIndex = 0;
        VulSemaphore** pWaitSemaphores = nullptr;
        u32 waitSemaphoreCount = 0;
    };

public:
    VulQueue(VulLogicDevice* pLogicDevice, u32 queueFamily);
    ~VulQueue() override;
    void Submit(const VulQueueSubmitInfo &vulSubmitInfo) const;
    void Wait() const;
    void Present(const VulQueuePresentInfo& info) const;

private:
    VulLogicDevice* m_pLogicDevice = nullptr;
    u32 m_queueFamily = 0;
};

USING_GPU_NAMESPACE_END
#endif //VULQUEUE_H