/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 23:00
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulkanPipeline.h"

#include "../../base/device/VulPhysicalDevice.h"
#include "../../base/pipeline/VulPipeline.h"
#include "../../base/pipeline/VulPipelineLayout.h"
#include "../../base/pipeline/VulShader.h"
#include "../VulkanDevice.h"
#include "../../base/descriptor/VulDescriptorSetLayout.h"
#include "../resource/ResourceCache.h"

USING_GPU_NAMESPACE_BEGIN

VulkanPipeline::VulkanPipeline(VulkanDevice* pDevice, const EnvInfo& envInfo) : m_pVulkanDevice(pDevice) {
	m_pVulkanDevice->AddRef();
	m_pShader = new VulShader(pDevice->GetLogicDevice(), envInfo.shaderInfo);

	m_pPipelineLayout = m_pShader->GetPipelineLayout();
	m_pPipelineLayout->AddRef();

	m_pDescriptorSetLayout = m_pShader->GetDescriptorSetLayout();
	m_pDescriptorSetLayout->AddRef();
}

VulkanPipeline::~VulkanPipeline() {
	m_pShader->SubRef();
	m_pPipelineLayout->SubRef();
	m_pDescriptorSetLayout->SubRef();
	m_pVulkanDevice->GetResourceCache()->DeleteAllDescriptorSet(this);
	m_pPipeline->SubRef();
	m_pVulkanDevice->SubRef();
}

USING_GPU_NAMESPACE_END