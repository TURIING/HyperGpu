/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/9 20:55
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulkanCmdManager.h"

#include "VulkanCmd.h"
#include "VulkanDevice.h"
#include "../base/VulCommandPool.h"
#include "../base/VulPhysicalDevice.h"

VulkanCmdManager::VulkanCmdManager(const std::shared_ptr<VulkanDevice>& device): m_pVulkanDevice(device) {
    m_pCmdPool = std::make_shared<VulCommandPool>(m_pVulkanDevice->GetLogicDevice(), m_pVulkanDevice->GetPhysicalDevice()->GetQueueFamilyIndices().graphicsFamily.value());
}

VulkanCmdManager::~VulkanCmdManager() {
}

std::shared_ptr<GpuCmd> VulkanCmdManager::CreateCommandBuffer() {
    return std::make_shared<VulkanCmd>(m_pVulkanDevice, m_pCmdPool);
}
