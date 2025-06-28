/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 23:00
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulkanPipeline.h"

#include "../base/device/VulPhysicalDevice.h"
#include "../base/device/VulLogicDevice.h"
#include "../base/descriptor/VulDescriptorPool.h"
#include "../base/descriptor/VulDescriptorSet.h"
#include "../base/pipeline/VulPipeline.h"
#include "../base/pipeline/VulPipelineLayout.h"
#include "../base/pipeline/VulRenderPass.h"
#include "../base/pipeline/VulShader.h"
#include "../base/resource/VulUniformBuffer.h"
#include "VulkanDevice.h"
#include "../base/descriptor/VulDescriptorSetLayout.h"
#include "resource/ResourceCache.h"
#include "resource/VulkanBuffer.h"
#include "resource/VulkanImage2D.h"

USING_GPU_NAMESPACE_BEGIN

VulkanPipeline::VulkanPipeline(VulkanDevice* pDevice, const RenderEnvInfo& renderEnvInfo) : m_pVulkanDevice(pDevice) {
	m_pVulkanDevice->AddRef();
	VulShader shader(pDevice->GetLogicDevice(), renderEnvInfo.shaderInfo);

	VulPipelineInputAssemblyState pipelineInputAssembly{renderEnvInfo.rasterInfo.primitiveType};
	VulPipelineRasterizationState pipelineRasterizationState = renderEnvInfo.rasterInfo;
	VulPipelineMultiSampleState	  pipelineMultiSampleState;
	VulPipelineColorBlendState	  pipelineColorBlendState;
	VulPipelineDynamicState		  pipelineDynamicState;
	VulPipelineDepthStencilState  pipelineDepthStencilState;

	std::vector<VulAttachmentInfo> attachmentInfos;
	attachmentInfos.reserve(renderEnvInfo.attachmentCount);
	for(auto i = 0; i < renderEnvInfo.attachmentCount; i++) {
		auto attachment = renderEnvInfo.pAttachment[i];
		attachmentInfos.push_back({
			attachment.type,
			attachment.index,
			gPixelFormatToVkFormat[static_cast<int>(attachment.format)],
		});
	}
	m_pRenderPass = m_pVulkanDevice->GetResourceCache()->RequestRenderPass({ attachmentInfos.data(), TO_U32(attachmentInfos.size()) });
	m_pRenderPass->AddRef();

	m_pPipelineLayout = shader.GetPipelineLayout();
	m_pPipelineLayout->AddRef();

	m_pDescriptorSetLayout = shader.GetDescriptorSetLayout();
	m_pDescriptorSetLayout->AddRef();

	auto			 vertexInputState = shader.GetPipelineVertexInputState();
	VulPipelineState pipelineState{
		.shaderStages		= shader.GetShaderStages(),
		.pipeLineLayout		= m_pPipelineLayout->GetHandle(),
		.renderPass			= m_pRenderPass->GetHandle(),
		.vertexInputState	= vertexInputState.GetCreateInfo(),
		.inputAssemblyState = pipelineInputAssembly.GetCreateInfo(),
		.viewportState		= VulPipelineViewportState::GetCreateInfo(),
		.rasterizationState = pipelineRasterizationState.GetCreateInfo(),
		.multiSampleState	= pipelineMultiSampleState.GetCreateInfo(),
		.depthStencilState	= pipelineDepthStencilState.GetCreateInfo(),
		.colorBlendState	= pipelineColorBlendState.GetCreateInfo(),
		.dynamicState		= pipelineDynamicState.GetCreateInfo(),
		.subPassIndex		= 0,
	};
	m_pPipeline = new VulPipeline(m_pVulkanDevice->GetLogicDevice(), pipelineState);

	VulDescriptorPoolCreateInfo descriptorPoolInfo{.poolSizes = {{VulDescriptorType::Sampler, 100}, {VulDescriptorType::UniformBuffer, 100}}};
	m_pDescriptorPool = new VulDescriptorPool(m_pVulkanDevice->GetLogicDevice(), descriptorPoolInfo);
	m_pDescriptorSet  = new VulDescriptorSet(m_pVulkanDevice->GetLogicDevice(), m_pDescriptorPool, m_pDescriptorSetLayout, shader.GetResourceBinding());
}

VulkanPipeline::~VulkanPipeline() {
	m_pPipelineLayout->SubRef();
	m_pDescriptorSetLayout->SubRef();
	m_pDescriptorSet->SubRef();
	m_pDescriptorPool->SubRef();
	m_pRenderPass->SubRef();
	m_pPipeline->SubRef();
	m_pVulkanDevice->SubRef();
}

void VulkanPipeline::SetUniformBuffers(GpuCmd::UniformBinding* infos, uint32_t count) const {
	std::vector<VulDescriptorSet::UniformBindingInfo> vecBindingInfo;
	vecBindingInfo.reserve(count);
	for (auto i = 0; i < count; i++) {
		vecBindingInfo.push_back({
			.pBufferInfo = dynamic_cast<VulUniformBuffer*>(infos[i].buffer)->GetDescriptorBufferInfo(),
			.name = infos[i].name,
		});
	}
	m_pDescriptorSet->SetUniformBuffer(vecBindingInfo);
}

void VulkanPipeline::SetImages(GpuCmd::ImageBinding* infos, uint32_t count) const {
	std::vector<VulDescriptorSet::ImageBindingInfo> vecBindingInfo;
	vecBindingInfo.reserve(count);
	for (auto i = 0; i < count; i++) {
		vecBindingInfo.push_back({
			.imageInfo = dynamic_cast<VulkanImage2D*>(infos[i].pImage)->GetDescriptorImageInfo(),
			.name = infos[i].name,
		});
	}
	m_pDescriptorSet->SetImage(vecBindingInfo);
};

USING_GPU_NAMESPACE_END