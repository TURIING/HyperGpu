/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/2/22 00:16
* @version: 1.0
* @description: 
********************************************************************************/

#include "VulkanDevice.h"

#include "../base/VulInstance.h"
#include "../base/VulLogicDevice.h"
#include "../base/VulPhysicalDevice.h"
#include "../base/surface/VulSurface.h"
#include "../base/surface/VulSwapChain.h"
#include "../base/sync/VulSemaphore.h"
#include "VulkanCmd.h"
#include "VulkanCmdManager.h"
#include "VulkanFence.h"
#include "VulkanPipeline.h"
#include "VulkanPipelineManager.h"
#include "VulkanResourceManager.h"
#include "VulkanSemaphore.h"
#include "VulkanSurface.h"
#include "VulkanSyncManager.h"

#ifdef NODEBUG
constexpr bool ENABLE_VALIDATION_LAYER = false;
#else
constexpr bool ENABLE_VALIDATION_LAYER = true;
#endif

VulkanDevice::VulkanDevice(const DeviceCreateInfo &info) {
	Singleton<LogManager>::GetInstance()->Init();

	m_pInstance = VulInstance::Builder()
					  .SetEnableValidationLayer(true)
					  .AddExtensions("VK_KHR_surface")
#if PLATFORM_WINDOWS
                    .AddExtensions("VK_KHR_win32_surface")
#elif PLATFORM_MACOS
                    .AddExtensions("VK_MVK_macos_surface")
                    .AddExtensions(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME)
                    .AddExtensions(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME)
#endif
                    .Build();

	m_pSurface = new VulSurface(m_pInstance, info.platformWindowInfo.handle);

	m_pPhysicalDevice = VulPhysicalDevice::Builder()
							.SetVulInstance(m_pInstance)
							.SetVulSurface(m_pSurface)
							.AddExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME)
#if PLATFORM_MACOS
							.AddExtension("VK_KHR_portability_subset")
#endif
							.Build();

	constexpr VkPhysicalDeviceFeatures deviceFeatures{
		.samplerAnisotropy = VK_TRUE,
	};
    m_pLogicDevice = VulLogicDevice::Builder()
                        .SetInstance(m_pInstance)
                        .SetSurface(m_pSurface)
                        .SetPhysicalDevice(m_pPhysicalDevice)
                        .AddExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME)
#if PLATFORM_MACOS
                        .AddExtension("VK_KHR_portability_subset")
#endif
                        .SetDeviceFeatures(deviceFeatures)
                        .Build();

	m_pCmdManager = new VulkanCmdManager(this);
	m_pLogicDevice->SetCmdManager(dynamic_cast<VulkanCmdManager*>(m_pCmdManager));
	m_pPipelineManager = new VulkanPipelineManager(this);
	m_pResourceManager = new VulkanResourceManager(this);
	m_pSyncManager	   = new VulkanSyncManager(this);
}

VulkanDevice::~VulkanDevice() {
	m_pInstance->SubRef();
	m_pSurface->SubRef();
	m_pPhysicalDevice->SubRef();
	m_pLogicDevice->SubRef();
	m_pCmdManager->SubRef();
	m_pPipelineManager->SubRef();
	m_pResourceManager->SubRef();
	m_pSyncManager->SubRef();
};

GpuSurface* VulkanDevice::GetSurface(Pipeline* pipeline) {
	return new VulkanSurface(this, dynamic_cast<VulkanPipeline*>(pipeline));
}

void VulkanDevice::Submit(GpuCmd* cmd, Semaphore* waitSemaphore, Semaphore* signalSemaphore, Fence* inFlightFence) {
	const auto command				 = dynamic_cast<VulkanCmd*>(cmd);
	const auto waitVulkanSemaphore	 = dynamic_cast<VulkanSemaphore*>(waitSemaphore);
	const auto signalVulkanSemaphore = dynamic_cast<VulkanSemaphore*>(signalSemaphore);
	const auto inFlightVulkanFence	 = dynamic_cast<VulkanFence*>(inFlightFence);
	command->Submit(waitVulkanSemaphore->GetHandle(), signalVulkanSemaphore->GetHandle(), inFlightVulkanFence->GetHandle());
}

void VulkanDevice::Present(Semaphore* waitSemaphore, GpuSurface* surface, uint32_t& imageIndex) {
	const auto	   waitVulkanSemaphore = dynamic_cast<VulkanSemaphore*>(waitSemaphore);
	const auto	   swapChain		   = dynamic_cast<VulkanSurface*>(surface)->GetSwapChain();
	VkSemaphore	   pSemaphore[]		   = {waitVulkanSemaphore->GetHandle()->GetHandle()};
	VkSwapchainKHR pSwapChain[]		   = {swapChain->GetHandle()};

	const VkPresentInfoKHR presentInfo = {
		.sType				= VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		.waitSemaphoreCount = 1,
		.pWaitSemaphores	= pSemaphore,
		.swapchainCount		= 1,
		.pSwapchains		= pSwapChain,
		.pImageIndices		= &imageIndex,
	};
	vkQueuePresentKHR(m_pLogicDevice->GetPresentQueue(), &presentInfo);
}
