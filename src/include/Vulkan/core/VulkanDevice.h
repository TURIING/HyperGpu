/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/2/22 00:09
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULKANDEVICE_H
#define VULKANDEVICE_H

#include "common/common.h"
#include "core/CoreDefine.h"

class VulSurface;
class VulInstance;

class VulkanDevice final : public Device {
public:
    explicit VulkanDevice(const DeviceCreateInfo &info);
    ~VulkanDevice() override;
    [[nodiscard]] std::shared_ptr<PipelineManager> GetPipelineManager() const override;

private:
    std::shared_ptr<VulInstance> m_pInstance;
    std::shared_ptr<VulSurface> m_pSurface;
};

#endif //VULKANDEVICE_H
