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
#include "../base/device/VulLogicDevice.h"
#include "../base/resource/VulImage2D.h"
#include "../base/surface/VulFrameBuffer.h"
#include "resource/ResourceCache.h"
#include "resource/VulkanImage2D.h"
#include "resource/VulkanInputAssembler.h"
USING_GPU_NAMESPACE_BEGIN

VulkanCmd::VulkanCmd(VulkanDevice* device, VulCommandPool* pool) : m_pVulkanDevice(device) {
	m_pVulkanDevice->AddRef();
	m_pCmd = new VulCommandBuffer(m_pVulkanDevice->GetLogicDevice(), pool);
}

VulkanCmd::~VulkanCmd() {
	m_pCmd->SubRef();
	m_pVulkanDevice->SubRef();
};

void VulkanCmd::Reset() {
	m_pCmd->Reset();
}

void VulkanCmd::Begin() {
	m_pCmd->BeginRecord();
}

void VulkanCmd::End() {
	m_pCmd->EndRecord();
}

void VulkanCmd::Draw(const DrawInfo& info) {
	LOG_ASSERT_INFO(m_pPipeline, "Pipeline is null, possibly BeginRenderPass be not called.")
	m_pPipeline->SetImages(info.pImageBinding, info.imageBindingCount);
	m_pPipeline->SetUniformBuffers(info.pUniformBinding, info.uniformBindingCount);

	auto pInputAssembler = dynamic_cast<VulkanInputAssembler*>(info.pInputAssembler);
	m_pCmd->BindVertexBuffer(pInputAssembler->GetVertexBuffer());
	const auto pIndexBuffer = pInputAssembler->GetIndexBuffer();

	if (pIndexBuffer) {
		m_pCmd->BindIndexBuffer(pIndexBuffer);
		m_pCmd->DrawIndex(pInputAssembler->GetIndexCount());
	}
	else {
		m_pCmd->Draw(pInputAssembler->GetVertexCount());
	}
}

void VulkanCmd::ClearColorImage(Image2D* image, const Color &color) {
	const auto pVulImage  = dynamic_cast<VulkanImage2D*>(image)->GetHandle();
	const auto colorValue = std::bit_cast<VkClearColorValue>(color);

	m_pCmd->TransitionImageLayout(pVulImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	m_pCmd->ClearColorForImage(pVulImage->GetHandle(), colorValue);
	m_pCmd->TransitionImageLayout(pVulImage, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

void VulkanCmd::BeginRenderPass(const BeginRenderInfo& beginRenderInfo) {
	m_pPipeline = dynamic_cast<VulkanPipeline*>(beginRenderInfo.pPipeline);
	const auto pRenderPass = m_pPipeline->GetRenderPass();

	VulRenderPassBeginInfo renderPassBeginInfo;
	for (const auto& value : beginRenderInfo.clearValue) {
		if (value.type == AttachmentType::COLOR) {
			renderPassBeginInfo.clearValues.push_back(
				VkClearValue{
					.color = {
						value.color.r,
						value.color.g,
						value.color.b,
						value.color.a
					}
				}
			);
		}
		else {
			renderPassBeginInfo.clearValues.push_back(
				VkClearValue{
					.depthStencil = {
						value.depthStencil.depth,
						value.depthStencil.stencil
					}
				}
			);
		}
	}
	renderPassBeginInfo.renderArea = {
		.offset = std::bit_cast<VkOffset2D>(beginRenderInfo.renderArea.offset),
		.extent = std::bit_cast<VkExtent2D>(beginRenderInfo.renderArea.size)
	};
	renderPassBeginInfo.pRenderPass = pRenderPass;

	if (beginRenderInfo.renderAttachmentType == RenderAttachmentType::Surface) {
		auto vulkanSurface = dynamic_cast<VulkanSurface*>(beginRenderInfo.surface);
		auto imageView = vulkanSurface->GetCurrentImage()->GetImageViewHandle();
		ResourceCache::FrameBufferCacheInfo frameBufferCacheInfo;
		frameBufferCacheInfo.attachmentCount = 1;
		frameBufferCacheInfo.pAttachments    = &imageView;
		frameBufferCacheInfo.pRenderPass     = pRenderPass;
		frameBufferCacheInfo.size            = vulkanSurface->GetSize();

		renderPassBeginInfo.pFrameBuffer = m_pVulkanDevice->GetResourceCache()->
		                                                    RequestFrameBuffer(frameBufferCacheInfo);
	}
	else if (beginRenderInfo.renderAttachmentType == RenderAttachmentType::Image2D) {
		auto                     [count, images] = beginRenderInfo.renderAttachment;
		std::vector<VkImageView> imageViews;
		imageViews.reserve(count);
		for (auto i = 0; i < count; i++) {
			imageViews.push_back(dynamic_cast<VulkanImage2D*>(images[i])->GetImageView());
		}

		ResourceCache::FrameBufferCacheInfo frameBufferCacheInfo;
		frameBufferCacheInfo.pRenderPass     = pRenderPass;
		frameBufferCacheInfo.attachmentCount = imageViews.size();
		frameBufferCacheInfo.pAttachments    = imageViews.data();
		frameBufferCacheInfo.size            = dynamic_cast<VulkanImage2D*>(images[0])->GetSize();

		renderPassBeginInfo.pFrameBuffer = m_pVulkanDevice->GetResourceCache()->
		                                                    RequestFrameBuffer(frameBufferCacheInfo);
	}

	m_pCmd->BeginRenderPass(renderPassBeginInfo);
	m_pCmd->BindPipeline(m_pPipeline->GetHandle());
	m_pCmd->BindDescriptorSets(VK_PIPELINE_BIND_POINT_GRAPHICS, m_pPipeline->GetPipelineLayout()->GetHandle(),
                               m_pPipeline->GetDescriptorSet()->GetHandle());
}

void VulkanCmd::EndRenderPass() {
	m_pCmd->EndRenderPass();
}

void VulkanCmd::SetViewport(const Viewport& viewport) {
	m_pCmd->SetViewport({transViewportToVkViewport(viewport)});
}

void VulkanCmd::SetScissor(const Scissor& scissor) {
	m_pCmd->SetScissor({transScissorToVkRect2D(scissor)});
}

void VulkanCmd::BlitImageToSurface(Image2D* pImage, GpuSurface* surface, ImageBlitRange *pRange, uint32_t rangeCount, Filter filter) {
	auto pVulImage        = dynamic_cast<VulkanImage2D*>(pImage)->GetHandle();
	auto pVulSurfaceImage = dynamic_cast<VulkanSurface*>(surface)->GetCurrentImage();

	VkImageSubresourceLayers subresourceLayers{
		.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
		.mipLevel = 0,
		.baseArrayLayer = 0,
		.layerCount = 1
	};

	std::vector<VkImageBlit> imageBlit;
	imageBlit.reserve(rangeCount);
	for (auto i = 0; i < rangeCount; i++) {
		const auto range = pRange[i];
		imageBlit.push_back({
			.srcSubresource = subresourceLayers,
			.srcOffsets = {
				{.x = range.srcArea.offset.x, .y = range.srcArea.offset.y, .z = 0},
				{
					.x = range.srcArea.offset.x + TO_I32(range.srcArea.size.width),
					.y = range.srcArea.offset.y + TO_I32(range.srcArea.size.height),
					.z = 1
				},
			},
			.dstSubresource = subresourceLayers,
			.dstOffsets = {
				{.x = range.dstArea.offset.x, .y = range.dstArea.offset.y, .z = 0},
				{
					.x = range.dstArea.offset.x + TO_I32(range.dstArea.size.width),
					.y = range.dstArea.offset.y + TO_I32(range.dstArea.size.height),
					.z = 1
				},
			},
		});
	}

	m_pCmd->TransitionImageLayout(pVulImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
	m_pCmd->TransitionImageLayout(pVulSurfaceImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	m_pCmd->BlitImage(pVulImage, pVulSurfaceImage, imageBlit, static_cast<VkFilter>(filter));
	m_pCmd->TransitionImageLayout(pVulImage, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	m_pCmd->TransitionImageLayout(pVulSurfaceImage, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
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
	return VkRect2D {
	.offset ={
		.x = scissor.offsetX,
		.y = scissor.offsetY,
	},
	.extent = {
		.width	= scissor.width,
		.height = scissor.height,
	}};
}

USING_GPU_NAMESPACE_END