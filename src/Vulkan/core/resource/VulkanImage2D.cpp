/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/15 12:22
 * @version: 1.0
 * @description:
 ********************************************************************************/
#include "VulkanImage2D.h"

#include "../../base/device/VulLogicDevice.h"
#include "../../base/command/VulCommandBuffer.h"
#include "../../base/resource/VulBuffer.h"
#include "../../base/resource/VulImage2D.h"
#include "../../base/resource/VulSampler.h"
#include "../VulkanDevice.h"
#include "VulkanSampler.h"
#include "../VulkanCmd.h"

USING_GPU_NAMESPACE_BEGIN
constexpr VkFormat gPixelFormatToVkFormat[] = {
	VK_FORMAT_R8G8B8A8_SRGB,		// R8G8B8A8
	VK_FORMAT_B8G8R8A8_SRGB,		// B8G8R8A8
};

VulkanImage2D::VulkanImage2D(VulkanDevice* device, const Image2DCreateInfo& info) : m_pVulkanDevice(device), m_size(info.size), m_usage(info.usage) {
	m_pVulkanDevice->AddRef();

	const VulImage2DCreateInfo vulImage2DCreateInfo{
		.size				 = info.size,
		.format				 = gPixelFormatToVkFormat[static_cast<int>(info.format)],
		.usage				 = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		.aspectFlags		 = transImageUsageToVkAspectFlag(info.usage),
		.memoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
	};
	m_pImage = new VulImage2D(m_pVulkanDevice->GetLogicDevice(), vulImage2DCreateInfo);

	m_pSampler = dynamic_cast<VulkanSampler*>(info.pSampler);
	m_pSampler->AddRef();

	m_imageInfo = VkDescriptorImageInfo{
		.sampler	 = m_pSampler->GetHandle()->GetHandle(),
		.imageView	 = m_pImage->GetImageViewHandle(),
		.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
	};
}

VulkanImage2D::~VulkanImage2D() {
	m_pSampler->SubRef();
	m_pImage->SubRef();
	m_pVulkanDevice->SubRef();
}

VkImageView VulkanImage2D::GetImageView() const {
	return m_pImage->GetImageViewHandle();
};

VkImageAspectFlags VulkanImage2D::transImageUsageToVkAspectFlag(ImageUsage usage) {
	switch(usage) {
		CASE_FROM_TO(ImageUsage::Color, VK_IMAGE_ASPECT_COLOR_BIT)
		CASE_FROM_TO(ImageUsage::Depth, VK_IMAGE_ASPECT_DEPTH_BIT)
		CASE_FROM_TO(ImageUsage::Stencil, VK_IMAGE_ASPECT_STENCIL_BIT)
	default:
		LOG_CRITICAL("Unrecognized image usage");
	}
}

void VulkanImage2D::FillPixels(GpuCmd* pCmd, const uint8_t* data, uint64_t dataSize) {
	const auto vulkanCmd = dynamic_cast<VulkanCmd*>(pCmd);
	const auto vulCmd = vulkanCmd->GetHandle();

	// 拷贝数据到暂存缓冲
	const auto stageBuffer = VulBuffer::Builder()
								 .SetSize(dataSize)
								 .SetUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT)
								 .SetProperties(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
								 .SetLogicDevice(m_pVulkanDevice->GetLogicDevice())
								 .Build();
	stageBuffer->MapData(0, dataSize, data);

	// 转换布局
	vulCmd->TransitionImageLayout(m_pImage,  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

	// 从暂存缓冲拷贝数据到image
	const VkBufferImageCopy region = {
		.bufferOffset	   = 0,
		.bufferRowLength   = 0,
		.bufferImageHeight = 0,
		.imageSubresource =
			{
				.aspectMask		= transImageUsageToVkAspectFlag(m_usage),
				.mipLevel		= 0,
				.baseArrayLayer = 0,
				.layerCount		= 1,
			},
		.imageOffset =
			{
				.x = 0,
				.y = 0,
				.z = 0,
			},
		.imageExtent =
			{
				.width	= m_size.width,
				.height = m_size.height,
				.depth	= 1,
			},
	};
	vulCmd->FillImageByBuffer(stageBuffer->GetHandle(), m_pImage->GetHandle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, region);

	// 再次转换布局
	vulCmd->TransitionImageLayout(m_pImage, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	stageBuffer->SubRef();
}
USING_GPU_NAMESPACE_END