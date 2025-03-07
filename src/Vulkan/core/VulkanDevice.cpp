/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/2/22 00:16
* @version: 1.0
* @description: 
********************************************************************************/

#include "VulkanDevice.h"

#include "../base/VulInstance.h"
#include "../base/VulSurface.h"
#include "../base/VulPhysicalDevice.h"
#include "../base/VulLogicDevice.h"
#include "VulkanPipelineManager.h"

#ifdef NODEBUG
constexpr bool ENABLE_VALIDATION_LAYER = false;
#else
constexpr bool ENABLE_VALIDATION_LAYER = true;
#endif

VulkanDevice::VulkanDevice(const DeviceCreateInfo &info) {
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

    m_pSurface = std::make_shared<VulSurface>(m_pInstance, info.platformWindowInfo.handle);

    m_pPhysicalDevice = VulPhysicalDevice::Builder()
                            .SetVulInstance(m_pInstance)
                            .SetVulSurface(m_pSurface)
                            .AddExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME)
#if PLATFORM_MACOS
                            .AddExtension("VK_KHR_portability_subset")
#endif
                            .Build();

    constexpr VkPhysicalDeviceFeatures deviceFeatures {
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

}

VulkanDevice::~VulkanDevice() {
}

std::shared_ptr<PipelineManager> VulkanDevice::GetPipelineManager() {
    return std::make_shared<VulkanPipelineManager>(shared_from_this());
}
