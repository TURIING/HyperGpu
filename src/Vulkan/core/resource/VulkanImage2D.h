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

USING_GPU_NAMESPACE_BEGIN

class VulCommandBuffer;
class VulkanDevice;
class VulImage2D;
class VulkanSampler;
using namespace HyperGpu;

class VulkanImage2D final : public Image2D {
public:
	VulkanImage2D(VulkanDevice* device, const Image2DCreateInfo& info);
	~VulkanImage2D() override;
	NODISCARD VulkanSampler* GetSampler() const { return m_pSampler; }
	NODISCARD VulImage2D* GetHandle() const { return m_pImage; }
	NODISCARD VkImageView GetImageView() const;
	NODISCARD VkDescriptorImageInfo GetDescriptorImageInfo();
	NODISCARD Size GetSize() const override { return m_size; }
	NODISCARD ImageAspectFlags GetAspectFlags() const override { return m_aspect; }
	ImageUsageFlags GetUsage() const override { return m_usage; }
	NODISCARD PixelFormat GetPixelFormat() const override { return m_pixelFormat; }
    uint32_t GetMipLevels() const override { return m_mipLevels; }

private:
	VulImage2D*			  m_pImage		  = nullptr;
	VulkanDevice*		  m_pVulkanDevice = nullptr;
	VulkanSampler*		  m_pSampler	  = nullptr;
	VkDescriptorImageInfo m_imageInfo{};
	Size				  m_size;
	ImageAspectFlags	  m_aspect;
	ImageUsageFlags		  m_usage;
    uint32_t			  m_mipLevels     = 0;
	PixelFormat			  m_pixelFormat;
};
USING_GPU_NAMESPACE_END
#endif // VULKANIMAGE2D_H
