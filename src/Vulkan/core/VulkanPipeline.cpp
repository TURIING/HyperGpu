/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 23:00
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulkanPipeline.h"

#include "../base/VulPhysicalDevice.h"
#include "../base/descriptor/VulDescriptorPool.h"
#include "../base/descriptor/VulDescriptorSet.h"
#include "../base/pipeline/VulPipeline.h"
#include "../base/pipeline/VulPipelineLayout.h"
#include "../base/pipeline/VulRenderPass.h"
#include "../base/pipeline/VulShader.h"
#include "../base/resource/VulUniformBuffer.h"
#include "VulkanDevice.h"
#include "../base/descriptor/VulDescriptorSetLayout.h"
#include "resource/VulkanBuffer.h"
#include "resource/VulkanImage2D.h"

VulkanPipeline::VulkanPipeline(VulkanDevice* pDevice, const RenderEnvInfo& renderEnvInfo) : m_pVulkanDevice(pDevice) {
	m_pVulkanDevice->AddRef();

	VulShader shader(pDevice->GetLogicDevice(), renderEnvInfo.shaderInfo);

	VulPipelineInputAssemblyState pipelineInputAssembly{renderEnvInfo.rasterInfo.primitiveType};
	VulPipelineRasterizationState pipelineRasterizationState = renderEnvInfo.rasterInfo;
	VulPipelineMultiSampleState	  pipelineMultiSampleState;
	VulPipelineColorBlendState	  pipelineColorBlendState;
	VulPipelineDynamicState		  pipelineDynamicState;
	VulPipelineDepthStencilState  pipelineDepthStencilState;

	auto renderPassBuilder = VulRenderPass::Builder().SetLogicDevice(m_pVulkanDevice->GetLogicDevice());
	for (auto i = 0; i < renderEnvInfo.attachmentCount; i++) {
		const auto attachment = renderEnvInfo.pAttachment[i];
		if(attachment.type == AttachmentType::COLOR) {
			renderPassBuilder.AddColorAttachment({attachment.index, m_pVulkanDevice->GetPhysicalDevice()->GetColorFormat()});
		} else if(attachment.type == AttachmentType::DEPTH) {
			renderPassBuilder.AddDepthAttachment({attachment.index, m_pVulkanDevice->GetPhysicalDevice()->GetDepthFormat()});
		}
	}
	m_pRenderPass = renderPassBuilder.Build();

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
		.colorBlendState	= pipelineColorBlendState.GetCreateInfo(),
		.viewportState		= VulPipelineViewportState::GetCreateInfo(),
		.depthStencilState	= pipelineDepthStencilState.GetCreateInfo(),
		.rasterizationState = pipelineRasterizationState.GetCreateInfo(),
		.inputAssemblyState = pipelineInputAssembly.GetCreateInfo(),
		.multiSampleState	= pipelineMultiSampleState.GetCreateInfo(),
		.dynamicState		= pipelineDynamicState.GetCreateInfo(),
		.subPassIndex		= 0,
	};
	m_pPipeline = new VulPipeline(m_pVulkanDevice->GetLogicDevice(), pipelineState);

	VulDescriptorPoolCreateInfo descriptorPoolInfo{.poolSizes = {{VulDescriptorType::Sampler, 100}, {VulDescriptorType::UniformBuffer, 100}}};
	m_pDescriptorPool = new VulDescriptorPool(m_pVulkanDevice->GetLogicDevice(), descriptorPoolInfo);
	m_pDescriptorSet  = new VulDescriptorSet(m_pVulkanDevice->GetLogicDevice(), m_pDescriptorPool, shader.GetDescriptorSetLayout());
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

void VulkanPipeline::SetUniformBuffers(Buffer** buffer, uint32_t count) {
	std::vector<VulUniformBuffer*> uniformBuffers;
	uniformBuffers.reserve(count);
	for (auto i = 0; i < count; i++) {
		uniformBuffers.push_back(dynamic_cast<VulkanBuffer*>(buffer[i])->GetUniformBuffer());
	}
	m_pDescriptorSet->SetUniformBuffer(uniformBuffers.data(), uniformBuffers.size());
}

void VulkanPipeline::SetImages(ImageBindingInfo* infos, uint32_t count) {
	std::vector<VulDescriptorSet::ImageBindingInfo> vecBindingInfo;
	vecBindingInfo.reserve(count);
	for (auto i = 0; i < count; i++) {
		vecBindingInfo.push_back({
			.imageInfo = dynamic_cast<VulkanImage2D*>(infos[i].pImage)->GetDescriptorImageInfo(),
			.binding = infos[i].binding,
		});
	}
	m_pDescriptorSet->SetImage(vecBindingInfo);
};
