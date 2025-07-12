/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 17:04
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulShaderModule.h"

#include "../device/VulLogicDevice.h"

USING_GPU_NAMESPACE_BEGIN

VulShaderModule::VulShaderModule(VulLogicDevice* device, const void *code, uint32_t size): m_pLogicDevice(device) {
	m_pLogicDevice->AddRef();
    VkShaderModuleCreateInfo shaderModuleCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = size,
        .pCode = static_cast<const uint32_t*>(code),
    };
    CALL_VK(vkCreateShaderModule(m_pLogicDevice->GetHandle(), &shaderModuleCreateInfo, nullptr, &m_pHandle));
    LOG_INFO("Shader module created successfully!");
}

VulShaderModule::~VulShaderModule() {
    vkDestroyShaderModule(m_pLogicDevice->GetHandle(), m_pHandle, nullptr);
	m_pLogicDevice->SubRef();
}

USING_GPU_NAMESPACE_END