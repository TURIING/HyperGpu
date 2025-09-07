/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 23:00
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulkanPipeline.h"

#include "../base/device/VulPhysicalDevice.h"
#include "../base/descriptor/VulDescriptorPool.h"
#include "../base/descriptor/VulDescriptorSet.h"
#include "../base/pipeline/VulPipeline.h"
#include "../base/pipeline/VulPipelineLayout.h"
#include "../base/pipeline/VulRenderPass.h"
#include "../base/pipeline/VulShader.h"
#include "VulkanDevice.h"
#include "../base/descriptor/VulDescriptorSetLayout.h"
#include "HyperGpu/src/Vulkan/base/device/VulLogicDevice.h"
#include "resource/ResourceCache.h"

USING_GPU_NAMESPACE_BEGIN

VulkanPipeline::VulkanPipeline(VulkanDevice* pDevice, const RenderEnvInfo& renderEnvInfo) : m_pVulkanDevice(pDevice) {
	this->renderEnvInfo = renderEnvInfo;
	m_pVulkanDevice->AddRef();
	m_pShader = new VulShader(pDevice->GetLogicDevice(), renderEnvInfo.shaderInfo);

	VulPipelineInputAssemblyState pipelineInputAssembly{renderEnvInfo.rasterInfo.primitiveType};
	VulPipelineRasterizationState pipelineRasterizationState(renderEnvInfo.rasterInfo);
	VulPipelineMultiSampleState	  pipelineMultiSampleState;
	VulPipelineColorBlendState	  pipelineColorBlendState(renderEnvInfo.blendInfo);
	VulPipelineDynamicState		  pipelineDynamicState;
	VulPipelineDepthStencilState  pipelineDepthStencilState;

	std::vector<VulAttachmentInfo> attachmentInfos;
	attachmentInfos.reserve(renderEnvInfo.attachments.size());
	for(auto attachment : renderEnvInfo.attachments) {
		attachmentInfos.push_back({
			attachment.type,
			attachment.index,
			gPixelFormatToVkFormat[TO_I32(attachment.format)],
			gAttachmentLoadOpToVkAttachmentLoadOp[TO_I32(attachment.loadOp)],
			gAttachmentStoreOpToVkAttachmentStoreOp[TO_I32(attachment.storeOp)],
		});
	}
	m_pRenderPass = m_pVulkanDevice->GetResourceCache()->RequestRenderPass({ attachmentInfos.data(), TO_U32(attachmentInfos.size()) });
	m_pRenderPass->AddRef();

	m_pPipelineLayout = m_pShader->GetPipelineLayout();
	m_pPipelineLayout->AddRef();

	m_pDescriptorSetLayout = m_pShader->GetDescriptorSetLayout();
	m_pDescriptorSetLayout->AddRef();

	auto vertexInputState = m_pShader->GetPipelineVertexInputState();
	VulPipelineState pipelineState{
		.shaderStages		= m_pShader->GetShaderStages(),
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
	m_pPipeline = new VulPipeline(m_pVulkanDevice->GetLogicDevice(), pipelineState, renderEnvInfo.objName);
}

VulkanPipeline::~VulkanPipeline() {
	m_pShader->SubRef();
	m_pPipelineLayout->SubRef();
	m_pDescriptorSetLayout->SubRef();
	m_pVulkanDevice->GetResourceCache()->DeleteAllDescriptorSet(this);
	m_pRenderPass->SubRef();
	m_pPipeline->SubRef();
	m_pVulkanDevice->SubRef();
}

USING_GPU_NAMESPACE_END