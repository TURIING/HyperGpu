/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/2/23 13:12
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULPHYSICALDEVICE_H
#define VULPHYSICALDEVICE_H

#include "../../common/common.h"
#include "VulObject.h"

class VulPhysicalDevice;
class VulInstance;
class VulSurface;

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities {};
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    [[nodiscard]] bool isComplete() const { return graphicsFamily.has_value() && presentFamily.has_value(); }
};

struct VulPhysicalDeviceCreateInfo {
    std::vector<const char*> extensions;
};

class VulPhysicalDeviceBuilder {
public:
    VulPhysicalDeviceBuilder& SetVulInstance(const std::shared_ptr<VulInstance> &pInstance) { m_pInstance = pInstance; return *this; }
    VulPhysicalDeviceBuilder& SetVulSurface(const std::shared_ptr<VulSurface> &pSurface) { m_pSurface = pSurface; return *this; }
    VulPhysicalDeviceBuilder& AddExtension(const char *ext);
    std::shared_ptr<VulPhysicalDevice> Build();

private:
    std::shared_ptr<VulInstance> m_pInstance;
    std::shared_ptr<VulSurface> m_pSurface;
    VulPhysicalDeviceCreateInfo m_createInfo;
};

class VulPhysicalDevice final : public VulObject<VkPhysicalDevice> {
public:
    VulPhysicalDevice(const std::shared_ptr<VulInstance> &pInstance, const std::shared_ptr<VulSurface> &pSurface, const VulPhysicalDeviceCreateInfo &createInfo);
    bool checkDeviceSupport(VkPhysicalDevice device, const std::vector<const char *>& extensions) const;
    static bool checkDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char *>& extensions);
    [[nodiscard]] SwapChainSupportDetails GetSwapChainSupportDetails() const { return m_swapChainSupportDetails; };
    [[nodiscard]] VkFormat GetColorFormat() const { return m_pSwapChainSurfaceFormat.format; }
    [[nodiscard]] VkFormat GetDepthFormat() const { return m_depthFormat; }
    [[nodiscard]] QueueFamilyIndices GetQueueFamilyIndices() const { return m_queueFamilyIndices; };
    [[nodiscard]] uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
    static VulPhysicalDeviceBuilder Builder() { return VulPhysicalDeviceBuilder{}; }

private:
    static SwapChainSupportDetails acquireSwapChainSupportDetails(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
    void chooseSwapSurfaceFormat();
    void acquireDepthFormatDetail();
    [[nodiscard]] static QueueFamilyIndices acquireQueueFamilyIndices(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

private:
    std::shared_ptr<VulInstance> m_pInstance;
    std::shared_ptr<VulSurface> m_pSurface;
    SwapChainSupportDetails m_swapChainSupportDetails;
    VkSurfaceFormatKHR m_pSwapChainSurfaceFormat {};
    VkFormat m_depthFormat = VK_FORMAT_UNDEFINED;
    QueueFamilyIndices m_queueFamilyIndices;
};

#endif //VULPHYSICALDEVICE_H
