/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/3 20:46
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulPipelineLayout.h"
#include "VulLogicDevice.h"

VulPipelineLayoutBuilder::VulPipelineLayoutBuilder() {
    m_pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
}

VulPipelineLayoutBuilder& VulPipelineLayoutBuilder::SetPushConstantRanges(const std::vector<VkPushConstantRange>& pushConstants) {
    m_pipelineLayoutCreateInfo.pushConstantRangeCount = pushConstants.size();
    m_pipelineLayoutCreateInfo.pPushConstantRanges = pushConstants.data();
    return *this;
}

VulPipelineLayoutBuilder& VulPipelineLayoutBuilder::SetDescriptorSetLayouts(const std::vector<VkDescriptorSetLayout>& descriptorLayouts) {
    m_pipelineLayoutCreateInfo.setLayoutCount = descriptorLayouts.size();
    m_pipelineLayoutCreateInfo.pSetLayouts = descriptorLayouts.data();
    return *this;
}

std::shared_ptr<VulPipeLineLayout> VulPipelineLayoutBuilder::Build() {
    return std::make_shared<VulPipeLineLayout>(m_pDevice, m_pipelineLayoutCreateInfo);
}

VulPipeLineLayout::VulPipeLineLayout(const std::shared_ptr<VulLogicDevice> &device, const VkPipelineLayoutCreateInfo &pipelineLayoutCreateInfo): m_pDevice(device) {
    CALL_VK(vkCreatePipelineLayout(m_pDevice->GetHandle(), &pipelineLayoutCreateInfo, nullptr, &m_pHandle));
    LOG_INFO("Pipeline Layout Created");
}

VulPipeLineLayout::~VulPipeLineLayout() {
    vkDestroyPipelineLayout(m_pDevice->GetHandle(), m_pHandle, nullptr);
}
