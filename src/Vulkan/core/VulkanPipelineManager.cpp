/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 23:26
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulkanPipelineManager.h"

#include "VulkanPipeline.h"
#include "VulkanDevice.h"

VulkanPipelineManager::VulkanPipelineManager(const std::shared_ptr<VulkanDevice>& device) : m_pDevice(device) {

}

std::shared_ptr<Pipeline> VulkanPipelineManager::CreateRenderPipeline(const RenderEnvInfo& renderEnvInfo) {
    return std::make_shared<VulkanPipeline>(m_pDevice, renderEnvInfo);
}
