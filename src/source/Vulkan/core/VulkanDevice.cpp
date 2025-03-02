/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/2/22 00:16
* @version: 1.0
* @description: 
********************************************************************************/

#include "Vulkan/core/VulkanDevice.h"

#include "Vulkan/base/VulInstance.h"
#include "Vulkan/base/VulSurface.h"
#include "Vulkan/base/VulPhysicalDevice.h"
#include "Vulkan/base/VulLogicDevice.h"
#include "Vulkan/core/VulkanPipelineManager.h"

#ifdef NODEBUG
constexpr bool ENABLE_VALIDATION_LAYER = false;
#else
constexpr bool ENABLE_VALIDATION_LAYER = true;
#endif

#if PLATFORM_WINDOWS
const std::vector<const char*> REQUIRE_DEVICE_EXTENSION = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
#elif PLATFORM_MACOS
const std::vector<const char*> REQUIRE_DEVICE_EXTENSION = { VK_KHR_SWAPCHAIN_EXTENSION_NAME, "VK_KHR_portability_subset" };
#endif

VulkanDevice::VulkanDevice(const DeviceCreateInfo &info) {
    m_pInstance = VulInstance::Builder()
                    .SetAppName(info.appName)
                    .SetAppVersion(info.appVersion)
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
