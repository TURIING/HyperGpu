/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/15 12:22
 * @version: 1.0
 * @description:
 ********************************************************************************/
#ifndef VULKANIMAGE2D_H
#define VULKANIMAGE2D_H

#include "../../../include/GpuResource.h"
#include "../../common/common.h"

class VulSample;
class VulkanDevice;
class VulImage2D;
using namespace HyperGpu;

class VulkanImage2D final : public Image2D {
public:
	 VulkanImage2D(VulkanDevice* device, const Image2DCreateInfo& info);
	~VulkanImage2D() override;

private:
	[[nodiscard]] static VkFormat			transPixelFormatToVkFormat(PixelFormat format);
	[[nodiscard]] static VkImageAspectFlags transImageUsageToVkAspectFlag(ImageUsage usage);
	[[nodiscard]] VkDescriptorImageInfo		GetDescriptorImageInfo() const { return m_imageInfo; }
	void									FillPixelData(const std::vector<uint8_t>& data) const;

private:
	VulImage2D*			  m_pImage		  = nullptr;
	VulkanDevice*		  m_pVulkanDevice = nullptr;
	VulSample*			  m_pSample		  = nullptr;
	VkDescriptorImageInfo m_imageInfo{};
	Size				  m_size;
	ImageUsage			  m_usage;
};

#endif // VULKANIMAGE2D_H
