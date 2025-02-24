/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/2/23 13:12
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULPHYSICALDEVICE_H
#define VULPHYSICALDEVICE_H

#include "common/common.h"
#include "VulObject.h"

class VulInstance;

struct VulPhysicalDeviceCreateInfo {
    std::vector<const char*> extensions;
};

class VulPhysicalDeviceBuilder {
public:
    VulPhysicalDeviceBuilder SetVulInstance(const std::shared_ptr<VulInstance> &pInstance) { m_pInstance = pInstance; return *this; }
    VulPhysicalDeviceBuilder AddExtension(const char *ext);

private:
    std::shared_ptr<VulInstance> m_pInstance;
    VulPhysicalDeviceCreateInfo m_createInfo;
};

class VulPhysicalDevice final : public VulObject<VkPhysicalDevice> {
public:
    VulPhysicalDevice(const std::shared_ptr<VulInstance> &pInstance, const VulPhysicalDeviceCreateInfo &createInfo);
    bool checkDeviceSupport(VkPhysicalDevice device, const std::vector<const char *>& extensions) const;
    static bool checkDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char *>& extensions);

private:
    std::shared_ptr<VulInstance> m_pInstance;
};

#endif //VULPHYSICALDEVICE_H
