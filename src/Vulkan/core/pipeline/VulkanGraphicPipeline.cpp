//
// Created by turiing on 2025/9/8.
//

#include "VulkanGraphicPipeline.h"

#include "../../base/device/VulPhysicalDevice.h"
#include "../../base/pipeline/VulPipeline.h"
#include "../../base/pipeline/VulPipelineLayout.h"
#include "../../base/pipeline/VulRenderPass.h"
#include "../../base/pipeline/VulShader.h"
#include "../VulkanDevice.h"
#include "../resource/ResourceCache.h"

USING_GPU_NAMESPACE_BEGIN

VulkanGraphicPipeline::VulkanGraphicPipeline(VulkanDevice* pDevice, const RenderEnvInfo& renderEnvInfo): VulkanPipeline(pDevice, renderEnvInfo) {
	this->renderEnvInfo = renderEnvInfo;
	m_pVulkanDevice->AddRef();

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

	auto vertexInputState = m_pShader->GetPipelineVertexInputState();
	VulGraphicPipelineState pipelineState{
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

	VulPipeline::VulPipelineCreateInfo pipelineCreateInfo;
	pipelineCreateInfo.graphicPipelineState = &pipelineState,
	pipelineCreateInfo.type = VulPipeline::Graphic,
	pipelineCreateInfo.objName = renderEnvInfo.objName;

	m_pPipeline = new VulPipeline(m_pVulkanDevice->GetLogicDevice(), pipelineCreateInfo);
}

VulkanGraphicPipeline::~VulkanGraphicPipeline() {
	m_pRenderPass->SubRef();
}

USING_GPU_NAMESPACE_END