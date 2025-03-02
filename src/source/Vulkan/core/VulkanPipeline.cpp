/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 23:00
* @version: 1.0
* @description: 
********************************************************************************/
#include "Vulkan/core/VulkanPipeline.h"
#include "Vulkan/base/VulShader.h"
#include "Vulkan/core/VulkanDevice.h"

VulkanPipeline::VulkanPipeline(const std::shared_ptr<VulkanDevice>& m_pDevice, const RenderEnvInfo& renderEnvInfo) {
    VulShader shader(m_pDevice->GetLogicDevice(), renderEnvInfo.vertexShader, renderEnvInfo.fragShader);

}
