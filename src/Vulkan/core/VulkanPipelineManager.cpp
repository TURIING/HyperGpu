/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 23:26
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulkanPipelineManager.h"

#include "VulkanPipeline.h"
#include "VulkanDevice.h"

VulkanPipelineManager::VulkanPipelineManager(VulkanDevice* device) : m_pDevice(device) {
	m_pDevice->AddRef();
}

VulkanPipelineManager::~VulkanPipelineManager() {
	m_pDevice->SubRef();
}

Pipeline* VulkanPipelineManager::CreateRenderPipeline(const RenderEnvInfo& renderEnvInfo) {
	return new VulkanPipeline(m_pDevice, renderEnvInfo);
}
