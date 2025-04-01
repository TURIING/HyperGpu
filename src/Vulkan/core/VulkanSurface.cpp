/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/16 15:18
 * @version: 1.0
 * @description:
 ********************************************************************************/
#include "VulkanSurface.h"

#include "../base/surface/VulFrameBuffer.h"
#include "../base/surface/VulSwapChain.h"
#include "VulkanDevice.h"
#include "VulkanPipeline.h"
#include "VulkanSemaphore.h"

VulkanSurface::VulkanSurface(VulkanDevice* device, VulkanPipeline* pipeline) : m_pVulkanDevice(device) {
	m_pVulkanDevice->AddRef();
	m_pSwapChain = new VulSwapChain(m_pVulkanDevice->GetLogicDevice(), m_pVulkanDevice->GetVulSurface());

	m_vecFrameBuffer.resize(m_pSwapChain->GetImageCount());
	for(auto i = 0; i < m_pSwapChain->GetImageCount(); i++) {
		m_vecFrameBuffer[i] = VulFrameBuffer::Builder()
								  .SetLogicDevice(m_pVulkanDevice->GetLogicDevice())
								  .SetRenderPass(pipeline->GetRenderPass())
								  .SetSize(m_pSwapChain->GetSize())
								  .AddAttachmentImageView(m_pSwapChain->GetImageView(i))
								  .Build();
	}
}

VulkanSurface::~VulkanSurface() {
	m_pSwapChain->SubRef();
	for(auto frameBuffer : m_vecFrameBuffer) {
		frameBuffer->SubRef();
	}
	m_pVulkanDevice->SubRef();
}

void VulkanSurface::AcquireNextImage(Semaphore* semaphore, uint32_t& imageIndex) {
	auto result = m_pSwapChain->AcquireNextImage(dynamic_cast<VulkanSemaphore*>(semaphore)->GetHandle(), imageIndex);
	if(result == VK_ERROR_OUT_OF_DATE_KHR) {
		LOG_ASSERT(false);
		// this->OnResize(m_size);
		// return false;
	} else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		LOG_CRITICAL("Failed to acquire swap chain image!");
	}
	m_frameIndex = (m_frameIndex + 1) % m_pSwapChain->GetImageCount();
};
