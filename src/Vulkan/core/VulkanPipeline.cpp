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
#include "../base/VulPhysicalDevice.h"
#include "../base/VulPipeline.h"
#include "../base/VulRenderPass.h"

VulkanPipeline::VulkanPipeline(const std::shared_ptr<VulkanDevice>& pDevice, const RenderEnvInfo& renderEnvInfo): m_pVulkanDevice(pDevice) {
    VulShader shader(pDevice->GetLogicDevice(), renderEnvInfo.shaderInfo);

    VulPipelineInputAssemblyState pipelineInputAssembly { renderEnvInfo.rasterInfo.primitiveType };
    VulPipelineRasterizationState pipelineRasterizationState = renderEnvInfo.rasterInfo;
    VulPipelineMultiSampleState pipelineMultiSampleState;
    VulPipelineColorBlendState pipelineColorBlendState;
    VulPipelineDynamicState pipelineDynamicState;
    VulPipelineDepthStencilState pipelineDepthStencilState;

    auto renderPassBuilder = VulRenderPass::Builder().SetLogicDevice(m_pVulkanDevice->GetLogicDevice());
    for(const auto &attachment : renderEnvInfo.attachments) {
        if(attachment.type == AttachmentType::COLOR) {
            renderPassBuilder.AddColorAttachment({ attachment.index, m_pVulkanDevice->GetPhysicalDevice()->GetColorFormat() });
        }
        else if(attachment.type == AttachmentType::DEPTH) {
            renderPassBuilder.AddDepthAttachment({ attachment.index, m_pVulkanDevice->GetPhysicalDevice()->GetDepthFormat() });
        }
    }
    m_pRenderPass = renderPassBuilder.Build();

    VulPipelineState pipelineState {
        .shaderStages = shader.GetShaderStages(),
        .pipeLineLayout = shader.GetPipelineLayout()->GetHandle(),
        .renderPass = m_pRenderPass->GetHandle(),
        .vertexInputState = shader.GetPipelineVertexInputState().GetCreateInfo(),
        .colorBlendState = pipelineColorBlendState.GetCreateInfo(),
        .viewportState = VulPipelineViewportState::GetCreateInfo(),
        .depthStencilState = pipelineDepthStencilState.GetCreateInfo(),
        .rasterizationState = pipelineRasterizationState.GetCreateInfo(),
        .inputAssemblyState = pipelineInputAssembly.GetCreateInfo(),
        .multiSampleState = pipelineMultiSampleState.GetCreateInfo(),
        .dynamicState = pipelineDynamicState.GetCreateInfo(),
        .subPassIndex = 0,
    };
    m_pPipeline = std::make_unique<VulPipeline>(m_pVulkanDevice->GetLogicDevice(), pipelineState);
}

VulkanPipeline::~VulkanPipeline() = default;
