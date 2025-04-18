/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/15 13:13
 * @version: 1.0
 * @description:
 ********************************************************************************/
#include "VulkanResourceManager.h"

#include "resource/VulkanBuffer.h"
#include "resource/VulkanImage2D.h"
#include "resource/VulkanSampler.h"

VulkanResourceManager::VulkanResourceManager(VulkanDevice* device) : m_pVulkanDevice(device) {
	m_pVulkanDevice->AddRef();
}

VulkanResourceManager::~VulkanResourceManager() {
	m_pVulkanDevice->SubRef();
}

Image2D* VulkanResourceManager::CreateImage2D(const Image2D::Image2DCreateInfo& info) {
	return new VulkanImage2D(m_pVulkanDevice, info);
}

Buffer* VulkanResourceManager::CreateBuffer(const BufferCreateInfo& createInfo) {
	return new VulkanBuffer(m_pVulkanDevice, createInfo);
}

Sampler* VulkanResourceManager::CreateSampler(const Sampler::SamplerCreateInfo& info) {
	return new VulkanSampler(m_pVulkanDevice, info);
}
