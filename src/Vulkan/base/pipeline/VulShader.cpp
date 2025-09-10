/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 17:16
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulShader.h"

#include "../device/VulLogicDevice.h"
#include "../descriptor/VulDescriptorSetLayout.h"
#include "GpuPipeline.h"
#include "spirv_reflect.h"
#include "VulPipelineLayout.h"
#include "VulShaderModule.h"

USING_GPU_NAMESPACE_BEGIN

VulShader::VulShader(VulLogicDevice* logicDevice, const ShaderInfo &shaderInfo): m_pLogicDevice(logicDevice) {
	m_pLogicDevice->AddRef();

    std::vector<VulDescriptorSetLayoutBindInfo> descriptorSetLayoutBindInfos;

    if (shaderInfo.pSpvVertexCode) {
        m_pVertexShaderModule = new VulShaderModule(logicDevice, shaderInfo.pSpvVertexCode, shaderInfo.spvVertexCodeSize);

        SpvReflectShaderModule spvReflectVertexShaderModule;
        auto result = spvReflectCreateShaderModule(shaderInfo.spvVertexCodeSize, shaderInfo.pSpvVertexCode, &spvReflectVertexShaderModule);
        LOG_ASSERT(result == SPV_REFLECT_RESULT_SUCCESS);

        m_shaderStages.emplace_back(makeShaderStage(spvReflectVertexShaderModule, m_pVertexShaderModule->GetHandle()));

        // 生成顶点描述
        this->computeInputState(spvReflectVertexShaderModule);

        computeDescriptorSetLayout(spvReflectVertexShaderModule, descriptorSetLayoutBindInfos);

        this->computePushConstant(spvReflectVertexShaderModule, VK_SHADER_STAGE_VERTEX_BIT);

        spvReflectDestroyShaderModule(&spvReflectVertexShaderModule);
    }

    if (shaderInfo.pSpvFragCode) {
        m_pFragShaderModule = new VulShaderModule(logicDevice, shaderInfo.pSpvFragCode, shaderInfo.spvFragCodeSize);

        SpvReflectShaderModule spvReflectFragShaderModule;
        auto result = spvReflectCreateShaderModule(shaderInfo.spvFragCodeSize, shaderInfo.pSpvFragCode, &spvReflectFragShaderModule);
        LOG_ASSERT(result == SPV_REFLECT_RESULT_SUCCESS);

        m_shaderStages.emplace_back(makeShaderStage(spvReflectFragShaderModule, m_pFragShaderModule->GetHandle()));

        computeDescriptorSetLayout(spvReflectFragShaderModule, descriptorSetLayoutBindInfos);

        this->computePushConstant(spvReflectFragShaderModule, VK_SHADER_STAGE_FRAGMENT_BIT);

        spvReflectDestroyShaderModule(&spvReflectFragShaderModule);
    }

    if (shaderInfo.pSpvCompCode) {
        m_pCompShaderModule = new VulShaderModule(logicDevice, shaderInfo.pSpvCompCode, shaderInfo.spvCompCodeSize);

        SpvReflectShaderModule spvReflectCompShaderModule;
        auto result = spvReflectCreateShaderModule(shaderInfo.spvCompCodeSize, shaderInfo.pSpvCompCode, &spvReflectCompShaderModule);
        LOG_ASSERT(result == SPV_REFLECT_RESULT_SUCCESS);

        m_shaderStages.emplace_back(makeShaderStage(spvReflectCompShaderModule, m_pCompShaderModule->GetHandle()));

        computeDescriptorSetLayout(spvReflectCompShaderModule, descriptorSetLayoutBindInfos);

        this->computePushConstant(spvReflectCompShaderModule, VK_SHADER_STAGE_FRAGMENT_BIT);

        spvReflectDestroyShaderModule(&spvReflectCompShaderModule);
    }

    // 生成描述符布局
    m_pDescriptorSetLayout = VulDescriptorSetLayout::Builder()
                                .SetLogicDevice(m_pLogicDevice)
                                .AddDescriptorBindings(descriptorSetLayoutBindInfos)
                                .Build();
}

VulShader::~VulShader() {
	m_pDescriptorSetLayout->SubRef();
	m_pVertexShaderModule->SubRef();
	m_pFragShaderModule->SubRef();
	m_pLogicDevice->SubRef();
};

void VulShader::computeInputState(const SpvReflectShaderModule& module) {
    uint32_t count = 0;
    auto result = spvReflectEnumerateInputVariables(&module, &count, nullptr);
    LOG_ASSERT(result == SPV_REFLECT_RESULT_SUCCESS);

    std::vector<SpvReflectInterfaceVariable*> inputVars(count);
    result = spvReflectEnumerateInputVariables(&module, &count, inputVars.data());
    LOG_ASSERT(result == SPV_REFLECT_RESULT_SUCCESS);

    auto &attributeDescription = m_pipelineVertexInputState.attributeDescriptions;
    attributeDescription.reserve(inputVars.size());

    m_pipelineVertexInputState.vecBindingDescription.push_back({
        .binding = 0,
        .stride = 0,
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
    });

    for (size_t index = 0; index < inputVars.size(); ++index) {
        const SpvReflectInterfaceVariable& var = *(inputVars[index]);

        // ignore built-in variables
        if (var.decoration_flags & SPV_REFLECT_DECORATION_BUILT_IN) {
            continue;
        }

        // vertex var
        if (auto varName = std::string(var.name); varName.starts_with('v')) {
            VkVertexInputAttributeDescription attr_desc{};
            attr_desc.location = var.location;
            attr_desc.binding = 0;
            attr_desc.format = static_cast<VkFormat>(var.format);
            attr_desc.offset = 0;  // final offset computed below after sorting.
            m_pipelineVertexInputState.attributeDescriptions.push_back(attr_desc);
        }
        // instance var
        else if (varName.starts_with('i')) {
            if (m_pipelineVertexInputState.vecBindingDescription.size() == 1) {
                m_pipelineVertexInputState.vecBindingDescription.push_back({
                    .binding = 1,
                    .stride = 0,
                    .inputRate = VK_VERTEX_INPUT_RATE_INSTANCE,
                });
            }
            auto &instanceBindingDescription = m_pipelineVertexInputState.vecBindingDescription[1];
            VkVertexInputAttributeDescription attr_desc{};
            attr_desc.location = var.location;
            attr_desc.binding = 1;
            attr_desc.format = static_cast<VkFormat>(var.format);
            attr_desc.offset = 0;
            m_pipelineVertexInputState.attributeDescriptions.push_back(attr_desc);
        }
        else {
            LOG_ASSERT_INFO(false, "Unrecognized vertex variable name.");
        }
    }

    // Sort attributes by location
    std::sort(std::begin(attributeDescription), std::end(attributeDescription), [](
        const VkVertexInputAttributeDescription& a, const VkVertexInputAttributeDescription& b) {
            return a.location < b.location;
        }
    );

    // Compute final offsets of each attribute, and total vertex stride.
    for (auto& attribute : attributeDescription) {
        uint32_t format_size = transformVkFormatToSize(attribute.format);
        auto &bindingDescription = m_pipelineVertexInputState.vecBindingDescription[attribute.binding];
        attribute.offset = bindingDescription.stride;
        bindingDescription.stride += format_size;
    }
}

void VulShader::computeDescriptorSetLayout(const SpvReflectShaderModule& module, std::vector<VulDescriptorSetLayoutBindInfo> &bindInfos) {
    uint32_t count = 0;
    auto result = spvReflectEnumerateDescriptorSets(&module, &count, nullptr);
    LOG_ASSERT(result == SPV_REFLECT_RESULT_SUCCESS);

    std::vector<SpvReflectDescriptorSet*> sets(count);
    result = spvReflectEnumerateDescriptorSets(&module, &count, sets.data());
    LOG_ASSERT(result == SPV_REFLECT_RESULT_SUCCESS);

    for (size_t indexSet = 0; indexSet < sets.size(); ++indexSet) {
        const SpvReflectDescriptorSet& reflectSet = *(sets[indexSet]);
        bindInfos.reserve(reflectSet.binding_count);
        for (uint32_t indexBinding = 0; indexBinding < reflectSet.binding_count; ++indexBinding) {
            const SpvReflectDescriptorBinding& reflectBinding = *(reflectSet.bindings[indexBinding]);
            VulDescriptorSetLayoutBindInfo bindInfo;
            bindInfo.bindIndex = reflectBinding.binding;
            bindInfo.descriptorCount = reflectBinding.count;
            bindInfo.descriptorType = static_cast<VkDescriptorType>(reflectBinding.descriptor_type);
            bindInfo.stageFlags = static_cast<VkShaderStageFlagBits>(module.shader_stage);
            bindInfos.push_back(bindInfo);

            m_mapResourceBinding[reflectBinding.name] = reflectBinding.binding;
        }
    }
}

void VulShader::computePushConstant(const SpvReflectShaderModule& module, VkShaderStageFlags stage) {
    uint32_t count = 0;
    auto result = spvReflectEnumeratePushConstantBlocks(&module, &count, nullptr);
    LOG_ASSERT(result == SPV_REFLECT_RESULT_SUCCESS);
    if (count == 0) { return; }

    std::vector<SpvReflectBlockVariable*> pushConstants(count);
    spvReflectEnumeratePushConstantBlocks(&module, &count, pushConstants.data());

    for (uint32_t i = 0; i < count; ++i) {
        SpvReflectBlockVariable* pushConstant = pushConstants[i];
        VkPushConstantRange pushConstantRange;
        pushConstantRange.offset = pushConstant->offset;
        pushConstantRange.size = pushConstant->size;
        pushConstantRange.stageFlags = stage;
        m_pushConstantRanges.push_back(pushConstantRange);
    }
}

VulPipeLineLayout* VulShader::GetPipelineLayout() const {
    return VulPipeLineLayout::Builder()
                                .SetPushConstantRanges(m_pushConstantRanges)
                                .SetLogicDevice(m_pLogicDevice)
                                .SetDescriptorSetLayouts({ m_pDescriptorSetLayout->GetHandle() })
                                .Build();
}

VkPipelineShaderStageCreateInfo VulShader::makeShaderStage(const SpvReflectShaderModule& module, VkShaderModule shaderModule) {
    return VkPipelineShaderStageCreateInfo {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage = transformSpvStageToVk(module.shader_stage),
        .module = shaderModule,
        .pName = "main"
    };
}

VkShaderStageFlagBits VulShader::transformSpvStageToVk(int spvStage) {
    switch (static_cast<SpvReflectShaderStageFlagBits>(spvStage)) {
    case SPV_REFLECT_SHADER_STAGE_VERTEX_BIT:           return VK_SHADER_STAGE_VERTEX_BIT;
    case SPV_REFLECT_SHADER_STAGE_FRAGMENT_BIT:         return VK_SHADER_STAGE_FRAGMENT_BIT;
    case SPV_REFLECT_SHADER_STAGE_GEOMETRY_BIT:         return VK_SHADER_STAGE_GEOMETRY_BIT;
    case SPV_REFLECT_SHADER_STAGE_COMPUTE_BIT:          return VK_SHADER_STAGE_COMPUTE_BIT;
    default:                                            LOG_ASSERT(false);
    }
}

uint32_t VulShader::transformVkFormatToSize(const VkFormat format) {
    switch (format) {
        CASE_FROM_TO(VK_FORMAT_UNDEFINED, 0)
        CASE_FROM_TO(VK_FORMAT_R4G4_UNORM_PACK8, 1)
        CASE_FROM_TO(VK_FORMAT_R4G4B4A4_UNORM_PACK16, 2)
        CASE_FROM_TO(VK_FORMAT_B4G4R4A4_UNORM_PACK16, 2)
        CASE_FROM_TO(VK_FORMAT_R5G6B5_UNORM_PACK16, 2)
        CASE_FROM_TO(VK_FORMAT_B5G6R5_UNORM_PACK16, 2)
        CASE_FROM_TO(VK_FORMAT_R5G5B5A1_UNORM_PACK16, 2)
        CASE_FROM_TO(VK_FORMAT_B5G5R5A1_UNORM_PACK16, 2)
        CASE_FROM_TO(VK_FORMAT_A1R5G5B5_UNORM_PACK16, 2)
        CASE_FROM_TO(VK_FORMAT_R8_UNORM, 1)
        CASE_FROM_TO(VK_FORMAT_R8_SNORM, 1)
        CASE_FROM_TO(VK_FORMAT_R8_USCALED, 1)
        CASE_FROM_TO(VK_FORMAT_R8_SSCALED, 1)
        CASE_FROM_TO(VK_FORMAT_R8_UINT, 1)
        CASE_FROM_TO(VK_FORMAT_R8_SINT, 1)
        CASE_FROM_TO(VK_FORMAT_R8_SRGB, 1)
        CASE_FROM_TO(VK_FORMAT_R8G8_UNORM, 2)
        CASE_FROM_TO(VK_FORMAT_R8G8_SNORM, 2)
        CASE_FROM_TO(VK_FORMAT_R8G8_USCALED, 2)
        CASE_FROM_TO(VK_FORMAT_R8G8_SSCALED, 2)
        CASE_FROM_TO(VK_FORMAT_R8G8_UINT, 2)
        CASE_FROM_TO(VK_FORMAT_R8G8_SINT, 2)
        CASE_FROM_TO(VK_FORMAT_R8G8_SRGB, 2)
        CASE_FROM_TO(VK_FORMAT_R8G8B8_UNORM, 3)
        CASE_FROM_TO(VK_FORMAT_R8G8B8_SNORM, 3)
        CASE_FROM_TO(VK_FORMAT_R8G8B8_USCALED, 3)
        CASE_FROM_TO(VK_FORMAT_R8G8B8_SSCALED, 3)
        CASE_FROM_TO(VK_FORMAT_R8G8B8_UINT, 3)
        CASE_FROM_TO(VK_FORMAT_R8G8B8_SINT, 3)
        CASE_FROM_TO(VK_FORMAT_R8G8B8_SRGB, 3)
        CASE_FROM_TO(VK_FORMAT_B8G8R8_UNORM, 3)
        CASE_FROM_TO(VK_FORMAT_B8G8R8_SNORM, 3)
        CASE_FROM_TO(VK_FORMAT_B8G8R8_USCALED, 3)
        CASE_FROM_TO(VK_FORMAT_B8G8R8_SSCALED, 3)
        CASE_FROM_TO(VK_FORMAT_B8G8R8_UINT, 3)
        CASE_FROM_TO(VK_FORMAT_B8G8R8_SINT, 3)
        CASE_FROM_TO(VK_FORMAT_B8G8R8_SRGB, 3)
        CASE_FROM_TO(VK_FORMAT_R8G8B8A8_UNORM, 4)
        CASE_FROM_TO(VK_FORMAT_R8G8B8A8_SNORM, 4)
        CASE_FROM_TO(VK_FORMAT_R8G8B8A8_USCALED, 4)
        CASE_FROM_TO(VK_FORMAT_R8G8B8A8_SSCALED, 4)
        CASE_FROM_TO(VK_FORMAT_R8G8B8A8_UINT, 4)
        CASE_FROM_TO(VK_FORMAT_R8G8B8A8_SINT, 4)
        CASE_FROM_TO(VK_FORMAT_R8G8B8A8_SRGB, 4)
        CASE_FROM_TO(VK_FORMAT_B8G8R8A8_UNORM, 4)
        CASE_FROM_TO(VK_FORMAT_B8G8R8A8_SNORM, 4)
        CASE_FROM_TO(VK_FORMAT_B8G8R8A8_USCALED, 4)
        CASE_FROM_TO(VK_FORMAT_B8G8R8A8_SSCALED, 4)
        CASE_FROM_TO(VK_FORMAT_B8G8R8A8_UINT, 4)
        CASE_FROM_TO(VK_FORMAT_B8G8R8A8_SINT, 4)
        CASE_FROM_TO(VK_FORMAT_B8G8R8A8_SRGB, 4)
        CASE_FROM_TO(VK_FORMAT_R16_UNORM, 2)
        CASE_FROM_TO(VK_FORMAT_R16_SNORM, 2)
        CASE_FROM_TO(VK_FORMAT_R16_USCALED, 2)
        CASE_FROM_TO(VK_FORMAT_R16_SSCALED, 2)
        CASE_FROM_TO(VK_FORMAT_R16_UINT, 2)
        CASE_FROM_TO(VK_FORMAT_R16_SINT, 2)
        CASE_FROM_TO(VK_FORMAT_R16_SFLOAT, 2)
        CASE_FROM_TO(VK_FORMAT_R32_UINT, 4)
        CASE_FROM_TO(VK_FORMAT_R32_SINT, 4)
        CASE_FROM_TO(VK_FORMAT_R32_SFLOAT, 4)
        CASE_FROM_TO(VK_FORMAT_R64_UINT, 8)
        CASE_FROM_TO(VK_FORMAT_R64_SINT, 8)
        CASE_FROM_TO(VK_FORMAT_R64_SFLOAT, 8)
    CASE_FROM_TO(VK_FORMAT_R32G32_SFLOAT, 8)
        CASE_FROM_TO(VK_FORMAT_R32G32B32A32_UINT, 16)
        CASE_FROM_TO(VK_FORMAT_R32G32B32A32_SINT, 16)
        CASE_FROM_TO(VK_FORMAT_R32G32B32A32_SFLOAT, 16)
        CASE_FROM_TO(VK_FORMAT_R32G32B32_SFLOAT, 12)
        CASE_FROM_TO(VK_FORMAT_R64G64B64A64_UINT, 32)
        CASE_FROM_TO(VK_FORMAT_R64G64B64A64_SINT, 32)
        CASE_FROM_TO(VK_FORMAT_R64G64B64A64_SFLOAT, 32)
        CASE_FROM_TO(VK_FORMAT_B10G11R11_UFLOAT_PACK32, 4)
        CASE_FROM_TO(VK_FORMAT_E5B9G9R9_UFLOAT_PACK32, 4)
        default:         LOG_ASSERT(false);
    }
}

USING_GPU_NAMESPACE_END