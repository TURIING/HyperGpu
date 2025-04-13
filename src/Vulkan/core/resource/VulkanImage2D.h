/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/15 12:22
 * @version: 1.0
 * @description:
 ********************************************************************************/
#ifndef VULKANIMAGE2D_H
#define VULKANIMAGE2D_H

#include "GpuResource.h"
#include "../../../common/common.h"

class VulCommandBuffer;
class VulkanDevice;
class VulImage2D;
class VulkanSampler;
using namespace HyperGpu;

class VulkanImage2D final : public Image2D {
public:
	VulkanImage2D(VulkanDevice* device, const Image2DCreateInfo& info);
	~VulkanImage2D() override;
	[[nodiscard]] VulkanSampler* GetSampler() const { return m_pSampler; }
	[[nodiscard]] VulImage2D* GetHandle() const { return m_pImage; }
	[[nodiscard]] VkDescriptorImageInfo* GetDescriptorImageInfo() { return &m_imageInfo; }
	[[nodiscard]] Size GetSize() const { return m_size; }
	void FillPixels(GpuCmd* pCmd, const uint8_t* data, uint64_t dataSize) override;

 private:
	[[nodiscard]] static VkFormat			transPixelFormatToVkFormat(PixelFormat format);
	[[nodiscard]] static VkImageAspectFlags transImageUsageToVkAspectFlag(ImageUsage usage);
	[[nodiscard]] VkDescriptorImageInfo		GetDescriptorImageInfo() const { return m_imageInfo; }

private:
	VulImage2D*			  m_pImage		  = nullptr;
	VulkanDevice*		  m_pVulkanDevice = nullptr;
	VulkanSampler*		  m_pSampler	  = nullptr;
	VkDescriptorImageInfo m_imageInfo{};
	Size				  m_size;
	ImageUsage			  m_usage;
};

#endif // VULKANIMAGE2D_H
