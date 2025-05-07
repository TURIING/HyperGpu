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
#include "../base/surface/VulSurface.h"
#include "../base/device/VulPhysicalDevice.h"

VulkanSurface::VulkanSurface(VulkanDevice* device, HyperGpu::PlatformWindowInfo platformWindowInfo) : m_pVulkanDevice(device) {
	m_pVulkanDevice->AddRef();
	m_pSurface = new VulSurface(m_pVulkanDevice->GetInstance(), platformWindowInfo.handle);
	m_pSwapChain = new VulSwapChain(m_pVulkanDevice->GetLogicDevice(), m_pSurface);
	m_pImageAcquiredSemaphore = new VulkanSemaphore(m_pVulkanDevice);
}

VulkanSurface::~VulkanSurface() {
	m_pImageAcquiredSemaphore->SubRef();
	m_pSwapChain->SubRef();
	m_pVulkanDevice->SubRef();
}

Semaphore* VulkanSurface::AcquireNextImage(uint32_t& imageIndex) {
	auto result = m_pSwapChain->AcquireNextImage(m_pImageAcquiredSemaphore->GetHandle(), imageIndex);
	if(result == VK_ERROR_OUT_OF_DATE_KHR) {
		LOG_ASSERT(false);
		// this->OnResize(m_size);
		// return false;
	} else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		LOG_CRITICAL("Failed to acquire swap chain image!");
	}

	m_frameIndex = (m_frameIndex + 1) % m_pSwapChain->GetImageCount();

	return m_pImageAcquiredSemaphore;
}

VulImage2D* VulkanSurface::GetCurrentImage() const {
	return m_pSwapChain->GetImage(m_frameIndex);
}

Size VulkanSurface::GetSize() const {
	return m_pSwapChain->GetSize();
};