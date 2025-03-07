/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/2 14:36
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulDescriptorSetLayout.h"
#include "VulLogicDevice.h"

VulDescriptorSetLayout::VulDescriptorSetLayout(const std::shared_ptr<VulLogicDevice> &device, const VulDescriptorSetLayoutCreateInfo &createInfo): m_pDevice(device) {
    std::vector<VkDescriptorSetLayoutBinding> bindings;

    for(const auto & [bindIndex, descriptorType, shaderStage, descriptorCount] : createInfo.descriptorSetLayoutBindings) {
        VkDescriptorSetLayoutBinding binding {
            .binding = static_cast<uint32_t>(bindIndex),
            .descriptorType = gDescriptorTypeMap[descriptorType],
            .descriptorCount = descriptorCount,
            .stageFlags = shaderStage,
            .pImmutableSamplers = nullptr,
        };
        bindings.push_back(binding);
    }

    const VkDescriptorSetLayoutCreateInfo layoutCreateInfo {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = static_cast<uint32_t>(bindings.size()),
        .pBindings = bindings.data()
    };
    CALL_VK(vkCreateDescriptorSetLayout(m_pDevice->GetHandle(), &layoutCreateInfo, nullptr, &m_pHandle));
    LOG_INFO("Descriptor set layout created.");
}

VulDescriptorSetLayout::~VulDescriptorSetLayout() {
    vkDestroyDescriptorSetLayout(m_pDevice->GetHandle(), m_pHandle, nullptr);
}

VulDescriptorSetLayoutBuilder VulDescriptorSetLayout::Builder() {
    return VulDescriptorSetLayoutBuilder {};
}

VulDescriptorSetLayoutBuilder& VulDescriptorSetLayoutBuilder::AddDescriptorBinding(VulDescriptorSetLayoutBindInfo&& bindInfo) {
    m_createInfo.descriptorSetLayoutBindings.push_back(bindInfo);
    return *this;
}

VulDescriptorSetLayoutBuilder& VulDescriptorSetLayoutBuilder::AddDescriptorBindings(const std::vector<VulDescriptorSetLayoutBindInfo>& bindInfos) {
    m_createInfo.descriptorSetLayoutBindings = bindInfos;
    return *this;
}

std::shared_ptr<VulDescriptorSetLayout> VulDescriptorSetLayoutBuilder::Build() {
    return std::make_shared<VulDescriptorSetLayout>(m_pDevice, m_createInfo);
}
