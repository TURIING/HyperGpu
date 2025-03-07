/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 23:00
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulkanPipeline.h"

#include "../base/VulPipelineLayout.h"
#include "../base/VulShader.h"
#include "VulkanDevice.h"

VulkanPipeline::VulkanPipeline(const std::shared_ptr<VulkanDevice>& pDevice, const RenderEnvInfo& renderEnvInfo) {
    VulShader shader(pDevice->GetLogicDevice(), renderEnvInfo.shaderInfo);

    VulPipelineInputAssemblyState pipelineInputAssembly { renderEnvInfo.rasterInfo.primitiveType };
    VulPipelineViewportState pipelineViewportState;
    VulPipelineRasterizationState pipelineRasterizationState = renderEnvInfo.rasterInfo;
    VulPipelineMultiSampleState pipelineMultiSampleState;
    VulPipelineColorBlendState pipelineColorBlendState;
    VulPipelineDynamicState pipelineDynamicState;


    VulPipelineState pipelineState {
        .shaderStages = shader.GetShaderStages(),
        .pipeLineLayout = shader.GetPipelineLayout()->GetHandle(),
    };
}

VulkanPipeline::~VulkanPipeline() = default;
