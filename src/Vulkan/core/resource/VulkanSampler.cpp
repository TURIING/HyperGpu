/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/4/2 19:25
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulkanSampler.h"

#include "../VulkanDevice.h"
#include "../../base/resource/VulSampler.h"

VulkanSampler::VulkanSampler(VulkanDevice* device, const SamplerCreateInfo& info): m_pVulkanDevice(device) {
    m_pVulkanDevice->AddRef();
    m_pVulSampler = new VulSampler(m_pVulkanDevice->GetLogicDevice(), std::bit_cast<VulSamplerCreateInfo>(info));
}

VulkanSampler::~VulkanSampler() {
    m_pVulSampler->SubRef();
    m_pVulkanDevice->SubRef();
}
