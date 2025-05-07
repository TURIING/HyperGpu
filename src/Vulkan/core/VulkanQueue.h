/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/4/19 17:21
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULKANQUEUE_H
#define VULKANQUEUE_H

#include "../../common/common.h"
#include "GpuDevice.h"

class VulQueue;
class VulkanDevice;

class VulkanQueue final: public Queue {
public:
    VulkanQueue(VulkanDevice* pVulkanDevice, QueueType queueType);
    ~VulkanQueue() override;
    void Submit(const SubmitInfo& submitInfo) override;
    void Wait() override;
    void Present(const PresentInfo &presentInfo) override;

private:
    VulQueue* m_pVulQueue = nullptr;
    VulkanDevice* m_pVulkanDevice = nullptr;
};

#endif //VULKANQUEUE_H
