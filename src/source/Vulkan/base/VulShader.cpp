/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 17:16
* @version: 1.0
* @description: 
********************************************************************************/
#include "Vulkan/base/VulShader.h"
#include "SPIRV-Reflect/spirv_reflect.h"
#include "Vulkan/base/VulShaderModule.h"

#define FORMAT_SIZE_CASE(fmt, size) case fmt: return size;

VulShader::VulShader(const std::shared_ptr<VulLogicDevice> &logicDevice, const std::vector<char>& vertexCode, const std::vector<char>& fragCode) {
    VulShaderModule vertexShaderModule(logicDevice, vertexCode);
    VulShaderModule fragShaderModule(logicDevice, fragCode);

    SpvReflectShaderModule spvReflectVertexShaderModule;
    auto result = spvReflectCreateShaderModule(vertexCode.size(), vertexCode.data(), &spvReflectVertexShaderModule);
    LOG_ASSERT(result == SPV_REFLECT_RESULT_SUCCESS);

    SpvReflectShaderModule spvReflectFragShaderModule;
    result = spvReflectCreateShaderModule(fragCode.size(), fragCode.data(), &spvReflectFragShaderModule);
    LOG_ASSERT(result == SPV_REFLECT_RESULT_SUCCESS);

    m_shaderStages.emplace_back(makeShaderStage(spvReflectVertexShaderModule, vertexShaderModule.GetHandle()));
    m_shaderStages.emplace_back(makeShaderStage(spvReflectFragShaderModule, fragShaderModule.GetHandle()));

    this->computeInputState(spvReflectVertexShaderModule);
}


void VulShader::computeInputState(const SpvReflectShaderModule& module) {
    uint32_t count = 0;
    auto result = spvReflectEnumerateInputVariables(&module, &count, nullptr);
    LOG_ASSERT(result == SPV_REFLECT_RESULT_SUCCESS);

    std::vector<SpvReflectInterfaceVariable*> inputVars(count);
    result = spvReflectEnumerateInputVariables(&module, &count, inputVars.data());
    LOG_ASSERT(result == SPV_REFLECT_RESULT_SUCCESS);

    auto &bindingDescription = m_pipelineVertexInputState.bindingDescription;
    auto &attributeDescription = m_pipelineVertexInputState.attributeDescriptions;

    bindingDescription.binding = 0;
    bindingDescription.stride = 0;  // computed below
    bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    m_pipelineVertexInputState.attributeDescriptions.reserve(inputVars.size());

    for (size_t index = 0; index < inputVars.size(); ++index) {
        const SpvReflectInterfaceVariable& var = *(inputVars[index]);

        // ignore built-in variables
        if (var.decoration_flags & SPV_REFLECT_DECORATION_BUILT_IN) {
            continue;
        }

        VkVertexInputAttributeDescription attr_desc{};
        attr_desc.location = var.location;
        attr_desc.binding = bindingDescription.binding;
        attr_desc.format = static_cast<VkFormat>(var.format);
        attr_desc.offset = 0;  // final offset computed below after sorting.
        m_pipelineVertexInputState.attributeDescriptions.push_back(attr_desc);
    }

    // Sort attributes by location
    std::sort(std::begin(attributeDescription), std::end(attributeDescription),
              [](const VkVertexInputAttributeDescription& a, const VkVertexInputAttributeDescription& b) {
                return a.location < b.location;
              });

    // Compute final offsets of each attribute, and total vertex stride.
    for (auto& attribute : attributeDescription) {
        uint32_t format_size = transformVkFormatToSize(attribute.format);
        attribute.offset = bindingDescription.stride;
        bindingDescription.stride += format_size;
    }
}

VkPipelineShaderStageCreateInfo VulShader::makeShaderStage(const SpvReflectShaderModule& module, VkShaderModule shaderModule) {
    return VkPipelineShaderStageCreateInfo {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage = transformSpvStageToVk(module.shader_stage),
        .module = shaderModule,
        .pName = module.entry_point_name
    };
}

VkShaderStageFlagBits VulShader::transformSpvStageToVk(int spvStage) {
    switch (static_cast<SpvReflectShaderStageFlagBits>(spvStage)) {
    case SPV_REFLECT_SHADER_STAGE_VERTEX_BIT:           return VK_SHADER_STAGE_VERTEX_BIT;
    case SPV_REFLECT_SHADER_STAGE_FRAGMENT_BIT:         return VK_SHADER_STAGE_FRAGMENT_BIT;
    case SPV_REFLECT_SHADER_STAGE_GEOMETRY_BIT:         return VK_SHADER_STAGE_GEOMETRY_BIT;
    default:                                            LOG_ASSERT(false);
    }
    return VK_SHADER_STAGE_VERTEX_BIT;
}

uint32_t VulShader::transformVkFormatToSize(const VkFormat format) {
    switch (format) {
        FORMAT_SIZE_CASE(VK_FORMAT_UNDEFINED, 0)
        FORMAT_SIZE_CASE(VK_FORMAT_R4G4_UNORM_PACK8, 1)
        FORMAT_SIZE_CASE(VK_FORMAT_R4G4B4A4_UNORM_PACK16, 2)
        FORMAT_SIZE_CASE(VK_FORMAT_B4G4R4A4_UNORM_PACK16, 2)
        FORMAT_SIZE_CASE(VK_FORMAT_R5G6B5_UNORM_PACK16, 2)
        FORMAT_SIZE_CASE(VK_FORMAT_B5G6R5_UNORM_PACK16, 2)
        FORMAT_SIZE_CASE(VK_FORMAT_R5G5B5A1_UNORM_PACK16, 2)
        FORMAT_SIZE_CASE(VK_FORMAT_B5G5R5A1_UNORM_PACK16, 2)
        FORMAT_SIZE_CASE(VK_FORMAT_A1R5G5B5_UNORM_PACK16, 2)
        FORMAT_SIZE_CASE(VK_FORMAT_R8_UNORM, 1)
        FORMAT_SIZE_CASE(VK_FORMAT_R8_SNORM, 1)
        FORMAT_SIZE_CASE(VK_FORMAT_R8_USCALED, 1)
        FORMAT_SIZE_CASE(VK_FORMAT_R8_SSCALED, 1)
        FORMAT_SIZE_CASE(VK_FORMAT_R8_UINT, 1)
        FORMAT_SIZE_CASE(VK_FORMAT_R8_SINT, 1)
        FORMAT_SIZE_CASE(VK_FORMAT_R8_SRGB, 1)
        FORMAT_SIZE_CASE(VK_FORMAT_R8G8_UNORM, 2)
        FORMAT_SIZE_CASE(VK_FORMAT_R8G8_SNORM, 2)
        FORMAT_SIZE_CASE(VK_FORMAT_R8G8_USCALED, 2)
        FORMAT_SIZE_CASE(VK_FORMAT_R8G8_SSCALED, 2)
        FORMAT_SIZE_CASE(VK_FORMAT_R8G8_UINT, 2)
        FORMAT_SIZE_CASE(VK_FORMAT_R8G8_SINT, 2)
        FORMAT_SIZE_CASE(VK_FORMAT_R8G8_SRGB, 2)
        FORMAT_SIZE_CASE(VK_FORMAT_R8G8B8_UNORM, 3)
        FORMAT_SIZE_CASE(VK_FORMAT_R8G8B8_SNORM, 3)
        FORMAT_SIZE_CASE(VK_FORMAT_R8G8B8_USCALED, 3)
        FORMAT_SIZE_CASE(VK_FORMAT_R8G8B8_SSCALED, 3)
        FORMAT_SIZE_CASE(VK_FORMAT_R8G8B8_UINT, 3)
        FORMAT_SIZE_CASE(VK_FORMAT_R8G8B8_SINT, 3)
        FORMAT_SIZE_CASE(VK_FORMAT_R8G8B8_SRGB, 3)
        FORMAT_SIZE_CASE(VK_FORMAT_B8G8R8_UNORM, 3)
        FORMAT_SIZE_CASE(VK_FORMAT_B8G8R8_SNORM, 3)
        FORMAT_SIZE_CASE(VK_FORMAT_B8G8R8_USCALED, 3)
        FORMAT_SIZE_CASE(VK_FORMAT_B8G8R8_SSCALED, 3)
        FORMAT_SIZE_CASE(VK_FORMAT_B8G8R8_UINT, 3)
        FORMAT_SIZE_CASE(VK_FORMAT_B8G8R8_SINT, 3)
        FORMAT_SIZE_CASE(VK_FORMAT_B8G8R8_SRGB, 3)
        FORMAT_SIZE_CASE(VK_FORMAT_R8G8B8A8_UNORM, 4)
        FORMAT_SIZE_CASE(VK_FORMAT_R8G8B8A8_SNORM, 4)
        FORMAT_SIZE_CASE(VK_FORMAT_R8G8B8A8_USCALED, 4)
        FORMAT_SIZE_CASE(VK_FORMAT_R8G8B8A8_SSCALED, 4)
        FORMAT_SIZE_CASE(VK_FORMAT_R8G8B8A8_UINT, 4)
        FORMAT_SIZE_CASE(VK_FORMAT_R8G8B8A8_SINT, 4)
        FORMAT_SIZE_CASE(VK_FORMAT_R8G8B8A8_SRGB, 4)
        FORMAT_SIZE_CASE(VK_FORMAT_B8G8R8A8_UNORM, 4)
        FORMAT_SIZE_CASE(VK_FORMAT_B8G8R8A8_SNORM, 4)
        FORMAT_SIZE_CASE(VK_FORMAT_B8G8R8A8_USCALED, 4)
        FORMAT_SIZE_CASE(VK_FORMAT_B8G8R8A8_SSCALED, 4)
        FORMAT_SIZE_CASE(VK_FORMAT_B8G8R8A8_UINT, 4)
        FORMAT_SIZE_CASE(VK_FORMAT_B8G8R8A8_SINT, 4)
        FORMAT_SIZE_CASE(VK_FORMAT_B8G8R8A8_SRGB, 4)
        FORMAT_SIZE_CASE(VK_FORMAT_R16_UNORM, 2)
        FORMAT_SIZE_CASE(VK_FORMAT_R16_SNORM, 2)
        FORMAT_SIZE_CASE(VK_FORMAT_R16_USCALED, 2)
        FORMAT_SIZE_CASE(VK_FORMAT_R16_SSCALED, 2)
        FORMAT_SIZE_CASE(VK_FORMAT_R16_UINT, 2)
        FORMAT_SIZE_CASE(VK_FORMAT_R16_SINT, 2)
        FORMAT_SIZE_CASE(VK_FORMAT_R16_SFLOAT, 2)
        FORMAT_SIZE_CASE(VK_FORMAT_R32_UINT, 4)
        FORMAT_SIZE_CASE(VK_FORMAT_R32_SINT, 4)
        FORMAT_SIZE_CASE(VK_FORMAT_R32_SFLOAT, 4)
        FORMAT_SIZE_CASE(VK_FORMAT_R64_UINT, 8)
        FORMAT_SIZE_CASE(VK_FORMAT_R64_SINT, 8)
        FORMAT_SIZE_CASE(VK_FORMAT_R64_SFLOAT, 8)
        FORMAT_SIZE_CASE(VK_FORMAT_R32G32B32A32_UINT, 16)
        FORMAT_SIZE_CASE(VK_FORMAT_R32G32B32A32_SINT, 16)
        FORMAT_SIZE_CASE(VK_FORMAT_R32G32B32A32_SFLOAT, 16)
        FORMAT_SIZE_CASE(VK_FORMAT_R64G64B64A64_UINT, 32)
        FORMAT_SIZE_CASE(VK_FORMAT_R64G64B64A64_SINT, 32)
        FORMAT_SIZE_CASE(VK_FORMAT_R64G64B64A64_SFLOAT, 32)
        FORMAT_SIZE_CASE(VK_FORMAT_B10G11R11_UFLOAT_PACK32, 4)
        FORMAT_SIZE_CASE(VK_FORMAT_E5B9G9R9_UFLOAT_PACK32, 4)
        default:         LOG_ASSERT(false);
    }
}

