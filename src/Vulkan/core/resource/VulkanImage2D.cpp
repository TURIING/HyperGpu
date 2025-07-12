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
		.aspectFlags		 = gImageUsageToVkImageAspectFlag[static_cast<int>(info.usage)],
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
}

VkDescriptorImageInfo * VulkanImage2D::GetDescriptorImageInfo() {
	m_imageInfo.imageLayout = m_pImage->GetCurrentImageLayout();
	return &m_imageInfo;
}

USING_GPU_NAMESPACE_END