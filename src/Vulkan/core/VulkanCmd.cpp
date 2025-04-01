/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/8 15:33
* @version: 1.0
* @description: 
********************************************************************************/
#include "../../common/common.h"
#include "VulkanCmd.h"

#include "../base/command/VulCommandBuffer.h"
#include "../base/command/VulCommandPool.h"
#include "../base/descriptor/VulDescriptorSet.h"
#include "../base/pipeline/VulPipelineLayout.h"
#include "VulkanBuffer.h"
#include "VulkanDevice.h"
#include "VulkanPipeline.h"
#include "VulkanSurface.h"

VulkanCmd::VulkanCmd(VulkanDevice* device, VulCommandPool* pool) : m_pVulkanDevice(device) {
	m_pVulkanDevice->AddRef();
	m_pCmd = new VulCommandBuffer(m_pVulkanDevice->GetLogicDevice(), pool);
}

VulkanCmd::~VulkanCmd() {
	m_pCmd->SubRef();
	m_pVulkanDevice->SubRef();
};

void VulkanCmd::Begin(const BeginRenderInfo& beginRenderInfo) {
	m_isBegin = true;
	m_pCmd->Reset();
	m_pCmd->BeginRecord();
	const auto pPipeline = dynamic_cast<VulkanPipeline*>(beginRenderInfo.pipeline);

	VulRenderPassBeginInfo renderPassBeginInfo;
	for(const auto& value : beginRenderInfo.clearValue) {
		if(value.type == AttachmentType::COLOR) {
			renderPassBeginInfo.clearValues.push_back(VkClearValue{.color = {value.color.r, value.color.g, value.color.b, value.color.a}});
		} else {
			renderPassBeginInfo.clearValues.push_back(VkClearValue{.depthStencil = {value.depthStencil.depth, value.depthStencil.stencil}});
		}
	}
	renderPassBeginInfo.renderArea	= {.offset = std::bit_cast<VkOffset2D>(beginRenderInfo.renderArea.offset), .extent = std::bit_cast<VkExtent2D>(beginRenderInfo.renderArea.size)};
	renderPassBeginInfo.pRenderPass = pPipeline->GetRenderPass();
	if(beginRenderInfo.renderAttachmentType == RenderAttachmentType::Surface) {
		auto vulkanSurface				 = dynamic_cast<VulkanSurface*>(beginRenderInfo.surface);
		renderPassBeginInfo.pFrameBuffer = vulkanSurface->GetCurrentFrameBuffer();
	} else if(beginRenderInfo.renderAttachmentType == RenderAttachmentType::Image2D) {
		// todo
	}

	m_pCmd->BeginRenderPass(renderPassBeginInfo);
	m_pCmd->BindPipeline(pPipeline->GetHandle());
	m_pCmd->BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, pPipeline->GetPipelineLayout()->GetHandle(), pPipeline->GetDescriptorSet()->GetHandle());
	m_pCmd->SetViewport({transViewportToVkViewport(beginRenderInfo.viewport)});
	m_pCmd->SetScissor({transScissorToVkRect2D(beginRenderInfo.scissor)});
}

void VulkanCmd::End() {
	m_isBegin = false;
	m_pCmd->EndRenderPass();
	m_pCmd->EndRecord();
}

void VulkanCmd::Draw(const DrawInfo& info) {
	LOG_ASSERT_INFO(m_isBegin, "Cmd has not yet started recording");
	if(info.vertexBuffer) {
		m_pCmd->BindVertexBuffer(dynamic_cast<VulkanBuffer*>(info.vertexBuffer)->GetVertexBuffer());
	}

	if(info.indexBuffer) {
		m_pCmd->BindIndexBuffer(dynamic_cast<VulkanBuffer*>(info.indexBuffer)->GetIndexBuffer());
	}

	m_pCmd->Draw(3);
}
void VulkanCmd::Submit(VulSemaphore* waitSemaphore, VulSemaphore* signalSemaphore, VulFence* inFlightFence) const {
	m_pCmd->Submit(waitSemaphore, signalSemaphore, inFlightFence);
}

VkViewport VulkanCmd::transViewportToVkViewport(const Viewport& viewport) {
	return VkViewport{
		.x		  = viewport.x,
		.y		  = viewport.y,
		.width	  = viewport.width,
		.height	  = viewport.height,
		.minDepth = 0.0,
		.maxDepth = 1.0,
	};
}

VkRect2D VulkanCmd::transScissorToVkRect2D(const Scissor& scissor) {
	// clang-format off
	return VkRect2D {
	.offset ={
		.x = scissor.offsetX,
		.y = scissor.offsetY,
	},
	.extent = {
		.width	= scissor.width,
		.height = scissor.height,
	}};
	// clang-format on
}
