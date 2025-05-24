/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/2/22 00:16
* @version: 1.0
* @description: 
********************************************************************************/

#include "VulkanDevice.h"

#include "../base/device/VulInstance.h"
#include "../base/device/VulLogicDevice.h"
#include "../base/device/VulPhysicalDevice.h"
#include "../base/surface/VulSwapChain.h"
#include "../base/sync/VulSemaphore.h"
#include "VulkanCmd.h"
#include "VulkanCmdManager.h"
#include "VulkanFence.h"
#include "VulkanPipeline.h"
#include "VulkanPipelineManager.h"
#include "VulkanQueue.h"
#include "VulkanResourceManager.h"
#include "VulkanSemaphore.h"
#include "VulkanSurface.h"
#include "VulkanSyncManager.h"
#include "resource/ResourceCache.h"

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
#elif PLATFORM_IOS
                    .AddExtensions("VK_EXT_metal_surface")
                    .AddExtensions(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME)
                    .AddExtensions(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME)
#endif
                    .Build();


	m_pPhysicalDevice = VulPhysicalDevice::Builder()
							.SetVulInstance(m_pInstance)
							.AddExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME)
//#if PLATFORM_MACOS
							.AddExtension("VK_KHR_portability_subset")
//#endif
							.Build();

	constexpr VkPhysicalDeviceFeatures deviceFeatures{
		.samplerAnisotropy = VK_TRUE,
	};
    m_pLogicDevice = VulLogicDevice::Builder()
                        .SetInstance(m_pInstance)
                        .SetPhysicalDevice(m_pPhysicalDevice)
						.SetQueueInfos(info.pQueueInfo, info.queueInfoCount)
                        .AddExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME)
//#if PLATFORM_MACOS
                        .AddExtension("VK_KHR_portability_subset")
//#endif
                        .SetDeviceFeatures(deviceFeatures)
                        .Build();

	m_pResourceCache = new ResourceCache(this);
	m_pCmdManager = new VulkanCmdManager(this);
	m_pLogicDevice->SetCmdManager(dynamic_cast<VulkanCmdManager*>(m_pCmdManager));
	m_pPipelineManager = new VulkanPipelineManager(this);
	m_pResourceManager = new VulkanResourceManager(this);
	m_pSyncManager	   = new VulkanSyncManager(this);
}

VulkanDevice::~VulkanDevice() {
	m_pInstance->SubRef();
	m_pPhysicalDevice->SubRef();
	m_pLogicDevice->SubRef();
	m_pCmdManager->SubRef();
	m_pPipelineManager->SubRef();
	m_pResourceManager->SubRef();
	m_pSyncManager->SubRef();
};

GpuSurface* VulkanDevice::CreateSurface(const PlatformWindowInfo &platformWindowInfo) {
	return new VulkanSurface(this, platformWindowInfo);
}

Queue* VulkanDevice::CreateQueue(QueueType queueType) {
	return new VulkanQueue(this, queueType);
}