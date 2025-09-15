/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 23:26
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulkanPipelineManager.h"

#include "VulkanPipeline.h"
#include "../VulkanDevice.h"
#include "../resource/ResourceCache.h"

USING_GPU_NAMESPACE_BEGIN

VulkanPipelineManager::VulkanPipelineManager(VulkanDevice* device) : m_pDevice(device) {
	m_pDevice->AddRef();
}

VulkanPipelineManager::~VulkanPipelineManager() {
	m_pDevice->SubRef();
}

Pipeline* VulkanPipelineManager::CreateGraphicPipeline(const RenderEnvInfo& renderEnvInfo) {
	return m_pDevice->GetResourceCache()->RequestPipeline(renderEnvInfo);
}

Pipeline * VulkanPipelineManager::CreateComputePipeline(const ComputeEnvInfo& computeEnvInfo) {
	return m_pDevice->GetResourceCache()->RequestPipeline(computeEnvInfo);
}

USING_GPU_NAMESPACE_END
