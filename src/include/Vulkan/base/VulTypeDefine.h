/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 22:34
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULTYPEDEFINE_H
#define VULTYPEDEFINE_H

#include "common/common.h"

struct VulPipelineVertexInputState
{
    VkVertexInputBindingDescription bindingDescription;
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
    [[nodiscard]] VkPipelineVertexInputStateCreateInfo GetCreateInfo() const {
        VkPipelineVertexInputStateCreateInfo vertexInputState{};
        vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputState.vertexBindingDescriptionCount = 1;
        vertexInputState.pVertexBindingDescriptions = &bindingDescription;
        vertexInputState.vertexAttributeDescriptionCount = attributeDescriptions.size();
        vertexInputState.pVertexAttributeDescriptions = attributeDescriptions.data();
        return vertexInputState;
    }
};


struct VulPipelineState
{
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
    VkPipelineLayout pipeLineLayout{};
    VkRenderPass renderPass{};
    VkPipelineVertexInputStateCreateInfo vertexInputState{};
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState{};
    VkPipelineViewportStateCreateInfo viewportState{};
    VkPipelineRasterizationStateCreateInfo rasterizationState{};
    VkPipelineMultisampleStateCreateInfo multiSampleState{};
    VkPipelineDepthStencilStateCreateInfo depthStencilState{};
    VkPipelineColorBlendStateCreateInfo colorBlendState{};
    VkPipelineDynamicStateCreateInfo dynamicState{};
    uint32_t subPassIndex{ 0 };
};


enum class VulDescriptorType
{
    Sampler,
    UniformBuffer,
};

#endif //VULTYPEDEFINE_H
