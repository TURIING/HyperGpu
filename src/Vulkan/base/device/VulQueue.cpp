/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/2/24 23:02
* @version: 1.0
* @description: 
********************************************************************************/

#include "VulQueue.h"

#include "VulLogicDevice.h"
#include "VulPhysicalDevice.h"
#include "HyperGpu/src/Vulkan/base/surface/VulSwapChain.h"
#include "HyperGpu/src/Vulkan/base/sync/VulFence.h"
#include "HyperGpu/src/Vulkan/base/sync/VulSemaphore.h"

USING_GPU_NAMESPACE_BEGIN

VulQueue::VulQueue(VulLogicDevice* pLogicDevice, u32 queueFamily): m_pLogicDevice(pLogicDevice), m_queueFamily(queueFamily) {
	m_pLogicDevice->AddRef();
	vkGetDeviceQueue(pLogicDevice->GetHandle(), m_queueFamily, 0, &m_pHandle);
}

VulQueue::~VulQueue() {
	m_pLogicDevice->SubRef();
}

void VulQueue::Submit(const VulQueueSubmitInfo& vulSubmitInfo) const {
    std::vector<VkPipelineStageFlags> waitStages;
    waitStages.reserve(vulSubmitInfo.waitSemaphoreCount);
    for (auto i  = 0; i < vulSubmitInfo.waitSemaphoreCount; i++) {
        waitStages.push_back(vulSubmitInfo.waitDstStageMask);
    }

    const VkSubmitInfo submitInfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = vulSubmitInfo.waitSemaphoreCount,
        .pWaitSemaphores = vulSubmitInfo.pWaitSemaphores,
        .pWaitDstStageMask = waitStages.data(),
        .commandBufferCount = vulSubmitInfo.cmdCount,
        .pCommandBuffers = vulSubmitInfo.pCmd,
        .signalSemaphoreCount = vulSubmitInfo.signalSemaphoreCount,
        .pSignalSemaphores = vulSubmitInfo.pSignalSemaphores,
    };
    CALL_VK(vkQueueSubmit(m_pHandle, 1, &submitInfo, vulSubmitInfo.pFence));
}

void VulQueue::Wait() const {
    CALL_VK(vkQueueWaitIdle(m_pHandle));
}

void VulQueue::Present(const VulQueuePresentInfo& info) const {
    const VkSwapchainKHR pSwapChain[] = { info.pSwapChain->GetHandle() };
    std::vector<VkSemaphore> semaphores;
    semaphores.reserve(info.waitSemaphoreCount);
    for (auto i = 0; i < info.waitSemaphoreCount; i++) {
        semaphores.push_back(info.pWaitSemaphores[i]->GetHandle());
    }

    const VkPresentInfoKHR presentInfo = {
        .sType				= VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = TO_U32(semaphores.size()),
        .pWaitSemaphores	= semaphores.data(),
        .swapchainCount		= 1,
        .pSwapchains		= pSwapChain,
        .pImageIndices		= &info.imageIndex,
    };
    vkQueuePresentKHR(m_pHandle, &presentInfo);
}

USING_GPU_NAMESPACE_END