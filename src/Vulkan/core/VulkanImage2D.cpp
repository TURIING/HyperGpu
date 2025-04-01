/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/15 12:22
 * @version: 1.0
 * @description:
 ********************************************************************************/
#include "VulkanImage2D.h"

#include "../base/VulLogicDevice.h"
#include "../base/command/VulCommandBuffer.h"
#include "../base/resource/VulBuffer.h"
#include "../base/resource/VulImage2D.h"
#include "../base/resource/VulSample.h"
#include "VulkanDevice.h"

VulkanImage2D::VulkanImage2D(VulkanDevice* device, const Image2DCreateInfo& info) : m_pVulkanDevice(device), m_size(info.size), m_usage(info.usage) {
	m_pVulkanDevice->AddRef();

	VulImage2DCreateInfo vulImage2DCreateInfo{
		.size				 = info.size,
		.format				 = transPixelFormatToVkFormat(info.format),
		.usage				 = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		.aspectFlags		 = transImageUsageToVkAspectFlag(info.usage),
		.memoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
	};
	m_pImage = new VulImage2D(m_pVulkanDevice->GetLogicDevice(), vulImage2DCreateInfo);

	VulSampleCreateInfo vulSampleCreateInfo{.magFilter = VK_FILTER_NEAREST, .minFilter = VK_FILTER_LINEAR, .addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT};
	m_pSample = new VulSample(m_pVulkanDevice->GetLogicDevice(), vulSampleCreateInfo);

	m_imageInfo = VkDescriptorImageInfo{
		.sampler	 = m_pSample->GetHandle(),
		.imageView	 = m_pImage->GetImageViewHandle(),
		.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
	};
}

VulkanImage2D::~VulkanImage2D() {
	m_pSample->SubRef();
	m_pImage->SubRef();
	m_pVulkanDevice->SubRef();
};

VkFormat VulkanImage2D::transPixelFormatToVkFormat(PixelFormat format) {
	switch(format) {
		CASE_FROM_TO(PixelFormat::R8G8B8A8, VK_FORMAT_R8G8B8A8_SRGB)
	default:
		LOG_CRITICAL("Unrecognized pixel format");
	}
}

VkImageAspectFlags VulkanImage2D::transImageUsageToVkAspectFlag(ImageUsage usage) {
	switch(usage) {
		CASE_FROM_TO(ImageUsage::Color, VK_IMAGE_ASPECT_COLOR_BIT)
		CASE_FROM_TO(ImageUsage::Depth, VK_IMAGE_ASPECT_DEPTH_BIT)
		CASE_FROM_TO(ImageUsage::Stencil, VK_IMAGE_ASPECT_STENCIL_BIT)
	default:
		LOG_CRITICAL("Unrecognized image usage");
	}
}

void VulkanImage2D::FillPixelData(const std::vector<uint8_t>& data) const {
	// 拷贝数据到暂存缓冲
	const auto stageBuffer = VulBuffer::Builder()
								 .SetSize(data.size())
								 .SetUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT)
								 .SetProperties(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
								 .SetLogicDevice(m_pVulkanDevice->GetLogicDevice())
								 .Build();
	stageBuffer->MapData(0, data.size(), data.data());

	// 转换布局
	m_pImage->TransitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

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
	// clang-format off
	m_pVulkanDevice->GetLogicDevice()->WithSingleCmdBuffer([&](VulCommandBuffer* cmd) {
		cmd->FillImageByBuffer(stageBuffer->GetHandle(), m_pImage->GetHandle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, region);
	});
	// clang-format on

	// 再次转换布局
	m_pImage->TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	stageBuffer->SubRef();
}
