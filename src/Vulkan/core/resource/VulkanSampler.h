/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/4/2 19:25
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULKANSAMPLER_H
#define VULKANSAMPLER_H

#include "../../../common/common.h"
#include "GpuResource.h"

class VulSampler;
class VulkanDevice;

class VulkanSampler final : public Sampler {
public:
    VulkanSampler(VulkanDevice* device, const SamplerCreateInfo& info);
    ~VulkanSampler() override;
    [[nodiscard]] VulSampler* GetHandle() const { return m_pVulSampler; }

private:
    VulkanDevice* m_pVulkanDevice = nullptr;
    VulSampler*   m_pVulSampler   = nullptr;
};

#endif //VULKANSAMPLER_H
