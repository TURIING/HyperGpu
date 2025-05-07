/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 22:28
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulPipeline.h"
#include "../device/VulLogicDevice.h"

VulPipeline::VulPipeline(VulLogicDevice* device, const VulPipelineState& state): m_pDevice(device) {
	m_pDevice->AddRef();
    VkGraphicsPipelineCreateInfo pipelineCreateInfo {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount = static_cast<uint32_t>(state.shaderStages.size()),
        .pStages = state.shaderStages.data(),
        .pVertexInputState = &state.vertexInputState,
        .pInputAssemblyState = &state.inputAssemblyState,
        .pViewportState = &state.viewportState,
        .pRasterizationState = &state.rasterizationState,
        .pMultisampleState = &state.multiSampleState,
        .pDepthStencilState = &state.depthStencilState,
        .pColorBlendState = &state.colorBlendState,
        .pDynamicState = &state.dynamicState,
        .layout = state.pipeLineLayout,
        .renderPass = state.renderPass,
        .subpass = state.subPassIndex,
        .basePipelineHandle = VK_NULL_HANDLE,
    };
    CALL_VK(vkCreateGraphicsPipelines(m_pDevice->GetHandle(), nullptr, 1, &pipelineCreateInfo, nullptr, &m_pHandle));
    LOG_INFO("Pipeline created");
}

VulPipeline::~VulPipeline() {
    vkDestroyPipeline(m_pDevice->GetHandle(), m_pHandle, nullptr);
	m_pDevice->SubRef();
}
