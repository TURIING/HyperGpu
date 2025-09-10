//
// Created by turiing on 2025/9/9.
//

#include "VulkanComputePipeline.h"
#include "../VulkanDevice.h"
#include "../../base/pipeline/VulPipeline.h"
#include "../../base/pipeline/VulShader.h"
#include "../../base/pipeline/VulPipelineLayout.h"
USING_GPU_NAMESPACE_BEGIN
    VulkanComputePipeline::VulkanComputePipeline(VulkanDevice *pDevice, const ComputeEnvInfo &computeEnvInfo): VulkanPipeline(pDevice, computeEnvInfo) {
    this->computeEnvInfo = computeEnvInfo;

    VulComputePipelineState pipelineState {
        .pipeLineLayout = m_pPipelineLayout->GetHandle(),
        .shaderStage = m_pShader->GetShaderStages()[0],
    };

    VulPipeline::VulPipelineCreateInfo pipelineCreateInfo;
    pipelineCreateInfo.computePipelineState = &pipelineState;
    pipelineCreateInfo.type = VulPipeline::Compute,
    pipelineCreateInfo.objName = renderEnvInfo.objName;

    m_pPipeline = new VulPipeline(m_pVulkanDevice->GetLogicDevice(), pipelineCreateInfo);
}

VulkanComputePipeline::~VulkanComputePipeline() = default;

USING_GPU_NAMESPACE_END
