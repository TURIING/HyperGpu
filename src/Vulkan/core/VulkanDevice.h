/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/2/22 00:09
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULKANDEVICE_H
#define VULKANDEVICE_H

#include "../../common/common.h"
#include "GpuDevice.h"

using namespace HyperGpu;

class VulLogicDevice;
class VulSurface;
class VulInstance;
class VulPhysicalDevice;

class VulkanDevice final : public GpuDevice, public std::enable_shared_from_this<VulkanDevice> {
public:
    explicit VulkanDevice(const DeviceCreateInfo &info);
    ~VulkanDevice() override;
    [[nodiscard]] std::shared_ptr<PipelineManager> GetPipelineManager() override;
    [[nodiscard]] std::shared_ptr<VulPhysicalDevice> GetPhysicalDevice() const { return m_pPhysicalDevice; }
    [[nodiscard]] std::shared_ptr<VulLogicDevice> GetLogicDevice() const { return m_pLogicDevice; }
    [[nodiscard]] std::shared_ptr<GpuCmdManager> GetCmdManager() override { return m_pCmdManager; };

private:
    std::shared_ptr<VulInstance> m_pInstance;
    std::shared_ptr<VulSurface> m_pSurface;
    std::shared_ptr<VulPhysicalDevice> m_pPhysicalDevice;
    std::shared_ptr<VulLogicDevice> m_pLogicDevice;
    std::shared_ptr<GpuCmdManager> m_pCmdManager;
};

#endif //VULKANDEVICE_H
