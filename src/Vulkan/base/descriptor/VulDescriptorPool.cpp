/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/2 14:14
* @version: 1.0
* @description: 
********************************************************************************/

#include "VulDescriptorPool.h"
#include "../device/VulLogicDevice.h"

USING_GPU_NAMESPACE_BEGIN

constexpr uint32_t DESCRIPTOR_POOL_MAX_SET_COUNT = 100;


VulDescriptorPool::VulDescriptorPool(VulLogicDevice* device, const VulDescriptorPoolCreateInfo &info): m_pDevice(device) {
	m_pDevice->AddRef();
    std::vector<VkDescriptorPoolSize> poolSizes(info.poolSizes.size());

    for(auto i = 0; i < info.poolSizes.size(); ++i) {
        poolSizes[i] = {
            .type = info.poolSizes[i].type,
            .descriptorCount = info.poolSizes[i].count,
        };
    }

    const VkDescriptorPoolCreateInfo poolCreateInfo {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .maxSets = DESCRIPTOR_POOL_MAX_SET_COUNT,
        .poolSizeCount = static_cast<uint32_t>(poolSizes.size()),
        .pPoolSizes = poolSizes.data(),
    };
    CALL_VK(vkCreateDescriptorPool(m_pDevice->GetHandle(), &poolCreateInfo, nullptr, &m_pHandle));
    LOG_INFO("Created descriptor pool");
}

VulDescriptorPool::~VulDescriptorPool() {
    vkDestroyDescriptorPool(m_pDevice->GetHandle(), m_pHandle, nullptr);
	m_pDevice->SubRef();
}

VulDescriptorPoolBuilder::VulDescriptorPoolBuilder(VulLogicDevice* device): m_pDevice(device) {}

VulDescriptorPoolBuilder& VulDescriptorPoolBuilder::AddPoolSize(VulDescriptorPoolSizeInfo&& info) {
    m_createInfo.poolSizes.emplace_back(info);
    return *this;
}

VulDescriptorPool* VulDescriptorPoolBuilder::Build() {
    return new VulDescriptorPool(m_pDevice, m_createInfo);
}

USING_GPU_NAMESPACE_END