/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/16 15:18
 * @version: 1.0
 * @description:
 ********************************************************************************/
#ifndef VULKANSURFACE_H
#define VULKANSURFACE_H

#include "../../common/common.h"

USING_GPU_NAMESPACE_BEGIN

class VulImage2D;
class VulkanSemaphore;
class VulSurface;
class VulFrameBuffer;
class VulkanPipeline;
class VulSwapChain;
class VulkanDevice;

class VulkanSurface final : public HyperGpu::GpuSurface {
public:
	explicit VulkanSurface(VulkanDevice* device, HyperGpu::PlatformWindowInfo platformWindowInfo);
	~ VulkanSurface() override;
	Semaphore* AcquireNextImage(uint32_t& imageIndex) override;
	[[nodiscard]] VulSwapChain* GetSwapChain() const { return m_pSwapChain; }
	[[nodiscard]] VulImage2D* GetCurrentImage() const;
	[[nodiscard]] Size GetSize() const;

private:
	VulkanDevice* m_pVulkanDevice = nullptr;
	VulSurface*	m_pSurface		   = nullptr;
	VulSwapChain* m_pSwapChain	 = nullptr;
	std::vector<VulkanSemaphore*> m_vecImageAcquiredSemaphore;
	int32_t	m_frameIndex = -1;
};

USING_GPU_NAMESPACE_END
#endif // VULKANSURFACE_H
