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
#include "resource/VulkanBuffer.h"
#include "VulkanDevice.h"
#include "VulkanPipeline.h"
#include "VulkanSurface.h"
#include "../base/VulLogicDevice.h"
#include "../base/resource/VulImage2D.h"
#include "HyperGpu/src/Vulkan/base/surface/VulFrameBuffer.h"
#include "resource/VulkanImage2D.h"

// struct AttachmentCollect {
// 	std::vector<Image2D*> images;
//
// 	bool operator==(const AttachmentCollect& other) const {
// 		if (images.size() != other.images.size()) return false;
// 		for (auto image : other.images) {
// 			if (std::ranges::find(images, image) == images.end()) {
// 				return false;
// 			}
// 		}
// 		return true;
// 	}
// };
//
// namespace std
// {
// 	template <>
// 	struct hash<AttachmentCollect> {
// 		std::size_t operator()(const AttachmentCollect& p) const {
// 			return std::hash<int>()(p.x) ^ (std::hash<int>()(p.y) << 1); // 组合哈希
// 		}
// 	};
// }

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
		auto [count, images]    = beginRenderInfo.renderAttachment;
		auto frameBufferBuilder = VulFrameBuffer::Builder()
		                          .SetLogicDevice(m_pVulkanDevice->GetLogicDevice())
		                          .SetRenderPass(pPipeline->GetRenderPass());
		for (auto i = 0; i < count; ++i) {
			auto pImage = dynamic_cast<VulkanImage2D*>(images[i]);
			frameBufferBuilder.SetSize(pImage->GetSize());
			frameBufferBuilder.AddAttachmentImageView(pImage->GetHandle()->GetImageViewHandle());
		}
		auto pFrameBuffer                = frameBufferBuilder.Build();
		renderPassBeginInfo.pFrameBuffer = pFrameBuffer;

		// std::hash<std::tuple<Image2D*, >>
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

	if (info.inputAssembler->vertexBuffer) {
		m_pCmd->BindVertexBuffer(dynamic_cast<VulkanBuffer*>(info.inputAssembler->vertexBuffer)->GetVertexBuffer());
	}

	if (info.inputAssembler->indexBuffer) {
		m_pCmd->BindIndexBuffer(dynamic_cast<VulkanBuffer*>(info.inputAssembler->indexBuffer)->GetIndexBuffer());
	}

	switch (info.drawType) {
	case DrawType::Vertex: m_pCmd->Draw(info.inputAssembler->vertexCount);
		break;
	case DrawType::Index: m_pCmd->DrawIndex(info.inputAssembler->indexCount);
		break;
	default:
		LOG_ASSERT(false);
	}
}

void VulkanCmd::Submit(VulSemaphore* waitSemaphore, VulSemaphore* signalSemaphore, VulFence* inFlightFence) const {
	m_pCmd->Submit(waitSemaphore, signalSemaphore, inFlightFence);
}

void VulkanCmd::ClearColorImage(Image2D* image, Color color) {
	const auto pVulImage  = dynamic_cast<VulkanImage2D*>(image)->GetHandle();
	const auto colorValue = std::bit_cast<VkClearColorValue>(color);

	pVulImage->TransitionImageLayout(m_pCmd, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	m_pCmd->ClearColorForImage(pVulImage->GetHandle(), colorValue);
	pVulImage->TransitionImageLayout(m_pCmd, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
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
