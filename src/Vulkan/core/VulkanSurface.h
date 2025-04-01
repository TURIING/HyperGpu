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
#include <GpuSurface.h>

namespace HyperGpu {
class Semaphore;
}

class VulFrameBuffer;
class VulkanPipeline;
class VulSwapChain;
class VulkanDevice;

class VulkanSurface : public HyperGpu::GpuSurface {
public:
	explicit					  VulkanSurface(VulkanDevice* device, VulkanPipeline* pipeline);
	~							  VulkanSurface() override;
	void						  AcquireNextImage(Semaphore* semaphore, uint32_t& imageIndex) override;
	[[nodiscard]] VulFrameBuffer* GetCurrentFrameBuffer() const { return m_vecFrameBuffer[m_frameIndex]; }
	[[nodiscard]] VulSwapChain*	  GetSwapChain() const { return m_pSwapChain; }

private:
	VulkanDevice*				 m_pVulkanDevice = nullptr;
	VulSwapChain*				 m_pSwapChain	 = nullptr;
	std::vector<VulFrameBuffer*> m_vecFrameBuffer;
	int32_t						 m_frameIndex = -1;
};

#endif // VULKANSURFACE_H
