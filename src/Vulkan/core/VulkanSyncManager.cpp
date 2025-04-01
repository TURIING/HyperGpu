/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/16 16:38
 * @version: 1.0
 * @description:
 ********************************************************************************/
#include "VulkanSyncManager.h"

#include "VulkanFence.h"
#include "VulkanSemaphore.h"

VulkanSyncManager::VulkanSyncManager(VulkanDevice* device) : m_pVulkanDevice(device) {
	m_pVulkanDevice->AddRef();
}

VulkanSyncManager::~VulkanSyncManager() {
	m_pVulkanDevice->SubRef();
}

Fence* VulkanSyncManager::CreateFence() {
	return new VulkanFence(m_pVulkanDevice);
}

Semaphore* VulkanSyncManager::CreateSemaphore() {
	return new VulkanSemaphore(m_pVulkanDevice);
}
