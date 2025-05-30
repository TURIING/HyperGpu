/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 22:34
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULTYPEDEFINE_H
#define VULTYPEDEFINE_H

#include "../../common/common.h"
#include "GpuPipeline.h"

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

struct VulPipelineInputAssemblyState
{
    PrimitiveType primitiveType;

    [[nodiscard]] static VkPrimitiveTopology transformPrimitiveTypeToVk(PrimitiveType primitiveType) {
        switch (primitiveType) {
            CASE_FROM_TO(PrimitiveType::POINT, VK_PRIMITIVE_TOPOLOGY_POINT_LIST)
            CASE_FROM_TO(PrimitiveType::LINE, VK_PRIMITIVE_TOPOLOGY_LINE_LIST)
            CASE_FROM_TO(PrimitiveType::LINE_STRIP, VK_PRIMITIVE_TOPOLOGY_LINE_STRIP)
            CASE_FROM_TO(PrimitiveType::TRIANGLE, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
            CASE_FROM_TO(PrimitiveType::TRIANGLE_STRIP, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP)
            default:    LOG_ASSERT(false);
        }
    }

    [[nodiscard]] VkPipelineInputAssemblyStateCreateInfo GetCreateInfo() const {
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyState{};
        inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssemblyState.topology = transformPrimitiveTypeToVk(primitiveType);
        inputAssemblyState.primitiveRestartEnable = VK_FALSE;
        return inputAssemblyState;
    }
};

struct VulPipelineViewportState
{
    static VkPipelineViewportStateCreateInfo GetCreateInfo()
    {
        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.scissorCount = 1;
        return viewportState;
    }
};

struct VulPipelineRasterizationState
{
    VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL;
    VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT;
    float lineWidth = 1.0f;
    VkFrontFace frontFace = VK_FRONT_FACE_CLOCKWISE;

    VulPipelineRasterizationState(const RasterizationInfo &rasterizationInfo) {
        this->polygonMode = static_cast<VkPolygonMode>(rasterizationInfo.polygonMode);
        this->cullMode = static_cast<VkCullModeFlags>(rasterizationInfo.cullMode);
        this->lineWidth = rasterizationInfo.lineWidth;
        this->frontFace = static_cast<VkFrontFace>(rasterizationInfo.frontFace);
    }

    [[nodiscard]] VkPipelineRasterizationStateCreateInfo GetCreateInfo() const {
        VkPipelineRasterizationStateCreateInfo rasterizationState{};
        rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizationState.depthClampEnable = VK_FALSE;
        rasterizationState.rasterizerDiscardEnable = VK_FALSE;
        rasterizationState.polygonMode = polygonMode;
        rasterizationState.lineWidth = lineWidth;
        rasterizationState.cullMode = cullMode;
        rasterizationState.frontFace = frontFace;
        rasterizationState.depthBiasEnable = VK_FALSE;
        return rasterizationState;
    }
};

struct VulPipelineMultiSampleState
{
    bool enable = false;
    VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT;
    [[nodiscard]] VkPipelineMultisampleStateCreateInfo GetCreateInfo() const {
        VkPipelineMultisampleStateCreateInfo multiSampleState{};
        multiSampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multiSampleState.sampleShadingEnable = enable ? VK_TRUE : VK_FALSE;
        multiSampleState.rasterizationSamples = sampleCount;
        return multiSampleState;
    }
};

struct VulPipelineColorBlendState
{
    bool enable = false;
    VkPipelineColorBlendAttachmentState colorBlendAttachmentState {};

    [[nodiscard]] VkPipelineColorBlendStateCreateInfo GetCreateInfo()
    {
		colorBlendAttachmentState.colorWriteMask	  = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachmentState.blendEnable		  = enable ? VK_TRUE : VK_FALSE;
		colorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA; // 正确的枚举值
		colorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		colorBlendAttachmentState.colorBlendOp		  = VK_BLEND_OP_ADD;
		colorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachmentState.alphaBlendOp		  = VK_BLEND_OP_ADD;
		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType				= VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable		= VK_FALSE;
		colorBlending.logicOp			= VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount	= 1;
		colorBlending.pAttachments		= &colorBlendAttachmentState;
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;
		return colorBlending;
	}
};

struct VulPipelineDynamicState
{
    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,      //  允许通过vkCmdSetViewport动态修改视口
        VK_DYNAMIC_STATE_SCISSOR
    };

    [[nodiscard]] VkPipelineDynamicStateCreateInfo GetCreateInfo() const {
        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		dynamicState.pDynamicStates	   = dynamicStates.data();
		return dynamicState;
	}
};

struct VulPipelineDepthStencilState {
	bool		depthTestEnable	  = false;
	bool		depthWriteEnable  = false;
	VkCompareOp depthOpFunc		  = VK_COMPARE_OP_LESS;
	bool		stencilTestEnable = false;

	[[nodiscard]] VkPipelineDepthStencilStateCreateInfo GetCreateInfo() const {
		const VkPipelineDepthStencilStateCreateInfo depthStencilState{
			.sType				   = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
			.depthTestEnable	   = depthTestEnable ? VK_TRUE : VK_FALSE,
			.depthWriteEnable	   = depthWriteEnable ? VK_TRUE : VK_FALSE,
			.depthCompareOp		   = depthOpFunc,
			.depthBoundsTestEnable = VK_FALSE,
			.minDepthBounds		   = 0.0f,
			.maxDepthBounds		   = 1.0f,
			.stencilTestEnable	   = stencilTestEnable ? VK_TRUE : VK_FALSE,
		};
		return depthStencilState;
	}
};

enum class VulDescriptorType {
	Sampler = 1,
    UniformBuffer = 6,
};

static std::unordered_map<VulDescriptorType, VkDescriptorType> gDescriptorTypeMap = {
    { VulDescriptorType::Sampler, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER },
    { VulDescriptorType::UniformBuffer, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER },
};

#endif //VULTYPEDEFINE_H
