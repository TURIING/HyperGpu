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
    LOG_ASSERT_INFO(m_pHandle != VK_NULL_HANDLE, "failed to find a suitable GPU!");
}

bool VulPhysicalDevice::checkDeviceSupport(VkPhysicalDevice device, const std::vector<const char *>& extensions) const {
    const auto indices = QueueFamilyIndices::GetQueueFamilyIndices(device, m_pSurface->GetHandle());
    const auto extensionSupported = checkDeviceExtensionSupport(device, extensions);

    auto isSwapChainAdequate = false;
    if(extensionSupported) {
        const auto swapChainSupportDetails = SwapChainSupportDetails::GetSwapChainSupportDetails(device, m_pSurface->GetHandle());
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

VulPhysicalDeviceBuilder VulPhysicalDevice::Builder() {
    return VulPhysicalDeviceBuilder{};
}

std::shared_ptr<VulPhysicalDevice> VulPhysicalDeviceBuilder::Build() {
    return std::make_shared<VulPhysicalDevice>(m_pInstance, m_pSurface, m_createInfo);
}

VulPhysicalDeviceBuilder& VulPhysicalDeviceBuilder::AddExtension(const char* ext) {
    m_createInfo.extensions.push_back(ext);
    return *this;
}
