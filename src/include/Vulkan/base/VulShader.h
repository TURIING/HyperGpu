/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 17:16
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULSHADER_H
#define VULSHADER_H

#include "common/common.h"
#include "Vulkan/base/VulTypeDefine.h"

class VulLogicDevice;
struct SpvReflectShaderModule;

class VulShader final {
public:
    VulShader(const std::shared_ptr<VulLogicDevice> &logicDevice, const std::vector<char> &vertexCode, const std::vector<char> &fragCode);
    [[nodiscard]] std::vector<VkPipelineShaderStageCreateInfo> GetShaderStages() const { return m_shaderStages; }
    [[nodiscard]] VulPipelineVertexInputState GetPipelineVertexInputState() { return m_pipelineVertexInputState; }

private:
    [[nodiscard]] static VkPipelineShaderStageCreateInfo makeShaderStage(const SpvReflectShaderModule &module, VkShaderModule shaderModule);
    [[nodiscard]] static VkShaderStageFlagBits transformSpvStageToVk(int spvStage);
    [[nodiscard]] static uint32_t transformVkFormatToSize(VkFormat format);
    void computeInputState(const SpvReflectShaderModule &module);

private:
    std::vector<VkPipelineShaderStageCreateInfo> m_shaderStages;
    VulPipelineVertexInputState m_pipelineVertexInputState;
};
#endif //VULSHADER_H
