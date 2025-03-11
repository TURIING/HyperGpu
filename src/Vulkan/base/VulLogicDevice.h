/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 15:58
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULLOGICDEVICE_H
#define VULLOGICDEVICE_H

#include "VulObject.h"
#include "../../common/common.h"

class VulCommandBuffer;
class VulkanCmdManager;
class VulLogicDevice;
class VulSurface;
class VulPhysicalDevice;
class VulInstance;

struct VulLogicDeviceCreateInfo {
    std::vector<const char*> extensions;
    VkPhysicalDeviceFeatures deviceFeatures;
};

class VulLogicDeviceBuilder {
public:
    VulLogicDeviceBuilder& SetInstance(const std::shared_ptr<VulInstance>& instance) { m_pInstance = instance; return *this; }
    VulLogicDeviceBuilder& SetPhysicalDevice(const std::shared_ptr<VulPhysicalDevice>& physicalDevice) { m_pPhysicalDevice = physicalDevice; return *this; }
    VulLogicDeviceBuilder& SetSurface(const std::shared_ptr<VulSurface>& surface) { m_pSurface = surface; return *this; }
    VulLogicDeviceBuilder& AddExtension(const char *ext);
    VulLogicDeviceBuilder& SetDeviceFeatures(const VkPhysicalDeviceFeatures& deviceFeatures) { m_createInfo.deviceFeatures = deviceFeatures; return *this; }
    std::shared_ptr<VulLogicDevice> Build();

private:
    std::shared_ptr<VulInstance> m_pInstance;
    std::shared_ptr<VulPhysicalDevice> m_pPhysicalDevice;
    std::shared_ptr<VulSurface> m_pSurface;
    VulLogicDeviceCreateInfo m_createInfo {};
};

class VulLogicDevice final : public VulObject<VkDevice>{
public:
    VulLogicDevice(const std::shared_ptr<VulInstance> &pInstance, const std::shared_ptr<VulPhysicalDevice> &pPhysicalDevice, const std::shared_ptr<VulSurface>& surface, const VulLogicDeviceCreateInfo &info);
    ~VulLogicDevice() override;
    [[nodiscard]] std::shared_ptr<VulPhysicalDevice> GetPhysicalDevice() const { return m_pPhysicalDevice; }
    void SetCmdManager(const std::shared_ptr<VulkanCmdManager> &pCmdManager) { m_pCmdManager = pCmdManager; }
    void WithSingleCmdBuffer(const std::function<void(const std::shared_ptr<VulCommandBuffer> &cmd)> &func) const;
    [[nodiscard]] VkQueue GetGraphicsQueue() const { return m_pGraphicsQueue; }
    [[nodiscard]] VkQueue GetPresentQueue() const { return m_pPresentQueue; }
    static VulLogicDeviceBuilder Builder();

private:
    [[nodiscard]] bool checkPresentSupport(uint32_t queueFamilyIndex) const;

private:
    std::shared_ptr<VulInstance> m_pInstance;
    std::shared_ptr<VulPhysicalDevice> m_pPhysicalDevice;
    std::shared_ptr<VulSurface> m_pSurface;
    VkQueue m_pGraphicsQueue = nullptr;
    VkQueue m_pPresentQueue = nullptr;
    std::shared_ptr<VulkanCmdManager> m_pCmdManager;
};

#endif //VULLOGICDEVICE_H
