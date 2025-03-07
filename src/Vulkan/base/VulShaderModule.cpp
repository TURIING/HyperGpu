/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 17:04
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulShaderModule.h"

#include "VulLogicDevice.h"

VulShaderModule::VulShaderModule(const std::shared_ptr<VulLogicDevice> &device, const uint8_t *code, uint32_t size): m_pLogicDevice(device) {
    VkShaderModuleCreateInfo shaderModuleCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = size,
        .pCode = reinterpret_cast<const uint32_t *>(code)
    };
    CALL_VK(vkCreateShaderModule(m_pLogicDevice->GetHandle(), &shaderModuleCreateInfo, nullptr, &m_pHandle));
    LOG_INFO("Shader module created successfully!");
}

VulShaderModule::~VulShaderModule() {
    vkDestroyShaderModule(m_pLogicDevice->GetHandle(), m_pHandle, nullptr);
}