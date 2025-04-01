/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/15 13:13
 * @version: 1.0
 * @description:
 ********************************************************************************/
#include "VulkanResourceManager.h"

#include "VulkanBuffer.h"
#include "VulkanImage2D.h"

VulkanResourceManager::VulkanResourceManager(VulkanDevice* device) : m_pVulkanDevice(device) {
	m_pVulkanDevice->AddRef();
}

VulkanResourceManager::~VulkanResourceManager() {
	m_pVulkanDevice->SubRef();
}

Image2D* VulkanResourceManager::CreateImage2D(const Image2D::Image2DCreateInfo& info) {
	return new VulkanImage2D(m_pVulkanDevice, info);
}

Buffer* VulkanResourceManager::CreateBuffer(Buffer::BufferType type, const uint8_t* data, uint64_t size) {
	return new VulkanBuffer(m_pVulkanDevice, type, data, size);
}
