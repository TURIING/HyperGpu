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
    static VulPhysicalDeviceBuilder Builder();

private:
    std::shared_ptr<VulInstance> m_pInstance;
    std::shared_ptr<VulSurface> m_pSurface;
};

#endif //VULPHYSICALDEVICE_H
