/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/2/23 13:20
* @version: 1.0
* @description: 
********************************************************************************/

#include "VulPhysicalDevice.h"
#include "VulInstance.h"
#include "VulQueue.h"
#include "VulSwapChain.h"
#include "VulSurface.h"

VulPhysicalDevice::VulPhysicalDevice(const std::shared_ptr<VulInstance> &pInstance, const std::shared_ptr<VulSurface> &pSurface, const VulPhysicalDeviceCreateInfo &createInfo): m_pInstance(pInstance), m_pSurface(pSurface) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(m_pInstance->GetHandle(), &deviceCount, nullptr);
    LOG_ASSERT_INFO(deviceCount != 0, "Failed to find GPUs with Vulkan support!");

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(m_pInstance->GetHandle(), &deviceCount, devices.data());
    for(const auto &device : devices) {
        if(this->checkDeviceSupport(device, createInfo.extensions)) {
            m_pHandle = device;
            break;
        }
    }

    m_queueFamilyIndices = acquireQueueFamilyIndices(m_pHandle, m_pSurface->GetHandle());
    m_swapChainSupportDetails = acquireSwapChainSupportDetails(m_pHandle, m_pSurface->GetHandle());
    this->chooseSwapSurfaceFormat();
    this->acquireDepthFormatDetail();

    LOG_ASSERT_INFO(m_pHandle != VK_NULL_HANDLE, "failed to find a suitable GPU!");
}

bool VulPhysicalDevice::checkDeviceSupport(VkPhysicalDevice device, const std::vector<const char *>& extensions) const {
    const auto indices = acquireQueueFamilyIndices(device, m_pSurface->GetHandle());
    const auto extensionSupported = checkDeviceExtensionSupport(device, extensions);

    auto isSwapChainAdequate = false;
    if(extensionSupported) {
        const auto swapChainSupportDetails = acquireSwapChainSupportDetails(device, m_pSurface->GetHandle());
        isSwapChainAdequate = !swapChainSupportDetails.formats.empty() && !swapChainSupportDetails.presentModes.empty();
    }

    VkPhysicalDeviceFeatures supportFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportFeatures);

    return indices.isComplete() && extensionSupported && isSwapChainAdequate && supportFeatures.samplerAnisotropy;
}

bool VulPhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char *>& extensions) {
    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(extensions.begin(), extensions.end());
    for(const auto &extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

uint32_t VulPhysicalDevice::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const {
    // 查询物理设备可用的内存类型
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(m_pHandle, &memoryProperties);

    for(auto i = 0; i < memoryProperties.memoryTypeCount; i++) {
        if((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    LOG_CRITICAL("Failed to find suitable memory type.");
}

std::shared_ptr<VulPhysicalDevice> VulPhysicalDeviceBuilder::Build() {
    return std::make_shared<VulPhysicalDevice>(m_pInstance, m_pSurface, m_createInfo);
}

VulPhysicalDeviceBuilder& VulPhysicalDeviceBuilder::AddExtension(const char* ext) {
    m_createInfo.extensions.push_back(ext);
    return *this;
}

SwapChainSupportDetails VulPhysicalDevice::acquireSwapChainSupportDetails(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.capabilities);

    uint32_t formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
    if(formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
    if(presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

// 选择合适的表面格式
void VulPhysicalDevice::chooseSwapSurfaceFormat() {
    for(const auto &availableFormat : m_swapChainSupportDetails.formats) {
        // 1.表示我们以B，G，R和A的顺序,每个颜色通道用8位无符号整型数表示，总共每像素使用32位表示
        // 2.表示SRGB颜色空间是否被支持
        if(availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            m_pSwapChainSurfaceFormat = availableFormat;
            return;
        }
    }
    m_pSwapChainSurfaceFormat = m_swapChainSupportDetails.formats[0];
}

void VulPhysicalDevice::acquireDepthFormatDetail() {
    constexpr VkFormat formats[] = { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };
    for(const auto format : formats) {
        VkFormatProperties formatProperties;
        vkGetPhysicalDeviceFormatProperties(m_pHandle, format, &formatProperties);

        if(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
            m_depthFormat = format;
            break;
        }
    }
}

QueueFamilyIndices VulPhysicalDevice::acquireQueueFamilyIndices(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
    QueueFamilyIndices indices;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    for(int familyIndex = 0; const auto &queueFamily : queueFamilies) {
        if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = familyIndex;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, familyIndex, surface, &presentSupport);
        if(presentSupport) indices.presentFamily = familyIndex;

        if(indices.isComplete()) break;

        familyIndex++;
    }
    return indices;
}
