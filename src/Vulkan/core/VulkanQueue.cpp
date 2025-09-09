/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/4/19 17:21
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulkanQueue.h"

#include "VulkanCmd.h"
#include "VulkanDevice.h"
#include "VulkanFence.h"
#include "VulkanSemaphore.h"
#include "../base/device/VulQueue.h"
#include "../base/device/VulPhysicalDevice.h"
#include "../base/command/VulCommandBuffer.h"
#include "../base/sync/VulFence.h"
#include "../base/sync/VulSemaphore.h"
#include "VulkanSurface.h"

USING_GPU_NAMESPACE_BEGIN

VulkanQueue::VulkanQueue(VulkanDevice* pVulkanDevice, QueueType queueType): m_pVulkanDevice(pVulkanDevice) {
    m_pVulQueue = new VulQueue(
        m_pVulkanDevice->GetLogicDevice(),
        m_pVulkanDevice->GetPhysicalDevice()->GetQueueFamily(queueType)
    );
}

VulkanQueue::~VulkanQueue() {
    m_pVulQueue->SubRef();
}

void VulkanQueue::Submit(const SubmitInfo& submitInfo) {
    std::vector<VkSemaphore> waitSemaphores;
    waitSemaphores.reserve(submitInfo.waitSemaphoreCount);
    for(auto i = 0; i < submitInfo.waitSemaphoreCount; i++) {
        waitSemaphores.push_back(dynamic_cast<VulkanSemaphore*>(submitInfo.pWaitSemaphores[i])->GetHandle()->GetHandle());
    }

    std::vector<VkSemaphore> signalSemaphores;
    signalSemaphores.reserve(submitInfo.signalSemaphoreCount);
    for(auto i = 0; i < submitInfo.signalSemaphoreCount; i++) {
        signalSemaphores.push_back(dynamic_cast<VulkanSemaphore*>(submitInfo.pSignalSemaphores[i])->GetHandle()->GetHandle());
    }

    std::vector<VkCommandBuffer> commandBuffers;
    commandBuffers.reserve(submitInfo.cmdCount);
    for(auto i = 0; i < submitInfo.cmdCount; i++) {
        commandBuffers.push_back(dynamic_cast<VulkanCmd*>(submitInfo.pCmd[i])->GetHandle()->GetHandle());
    }

    VulQueue::VulQueueSubmitInfo vulSubmitInfo {
        .pWaitSemaphores = waitSemaphores.data(),
        .waitSemaphoreCount = TO_U32(waitSemaphores.size()),
        .pSignalSemaphores = signalSemaphores.data(),
        .signalSemaphoreCount = TO_U32(signalSemaphores.size()),
        .pFence = dynamic_cast<VulkanFence*>(submitInfo.pFence)->GetHandle()->GetHandle(),
        .pCmd = commandBuffers.data(),
        .cmdCount = TO_U32(commandBuffers.size()),
        .waitDstStageMask = static_cast<VkPipelineStageFlags>(submitInfo.waitDstStageMask),
    };
    m_pVulQueue->Submit(vulSubmitInfo);
}

void VulkanQueue::Wait() {
    m_pVulQueue->Wait();
}

void VulkanQueue::Present(const Queue::PresentInfo &presentInfo) {
    std::vector<VulSemaphore*> waitSemaphores;
    waitSemaphores.reserve(presentInfo.waitSemaphoreCount);
    for(auto i = 0; i < presentInfo.waitSemaphoreCount; i++) {
        waitSemaphores.push_back(dynamic_cast<VulkanSemaphore*>(presentInfo.pWaitSemaphores[i])->GetHandle());
    }

    VulQueue::VulQueuePresentInfo info {
        .pSwapChain = dynamic_cast<VulkanSurface*>(presentInfo.pSurface)->GetSwapChain(),
        .imageIndex = *presentInfo.imageIndex,
        .pWaitSemaphores = waitSemaphores.data(),
        .waitSemaphoreCount = TO_U32(waitSemaphores.size())
    };
    m_pVulQueue->Present(info);
}

USING_GPU_NAMESPACE_END