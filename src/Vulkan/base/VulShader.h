/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 17:16
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULSHADER_H
#define VULSHADER_H

#include "../../common/common.h"
#include "VulTypeDefine.h"

namespace HyperGpu
{
    struct ShaderInfo;
}

class VulPipeLineLayout;
class VulShaderModule;
struct VulDescriptorSetLayoutBindInfo;
class VulDescriptorSetLayout;
class VulLogicDevice;
struct SpvReflectShaderModule;

class VulShader final {
public:
    VulShader(const std::shared_ptr<VulLogicDevice> &logicDevice, const ShaderInfo &shaderInfo);
    ~VulShader();
    [[nodiscard]] std::vector<VkPipelineShaderStageCreateInfo> GetShaderStages() const { return m_shaderStages; }
    [[nodiscard]] VulPipelineVertexInputState GetPipelineVertexInputState() { return m_pipelineVertexInputState; }
    [[nodiscard]] std::shared_ptr<VulDescriptorSetLayout> GetDescriptorSetLayout() const { return m_pDescriptorSetLayout; }
    [[nodiscard]] std::vector<VkPushConstantRange> GetPushConstantRanges() const { return m_pushConstantRanges; }
    [[nodiscard]] std::shared_ptr<VulPipeLineLayout> GetPipelineLayout() const;

private:
    [[nodiscard]] static VkPipelineShaderStageCreateInfo makeShaderStage(const SpvReflectShaderModule &module, VkShaderModule shaderModule);
    [[nodiscard]] static VkShaderStageFlagBits transformSpvStageToVk(int spvStage);
    [[nodiscard]] static uint32_t transformVkFormatToSize(VkFormat format);
    void computeInputState(const SpvReflectShaderModule &module);
    static void computeDescriptorSetLayout(const SpvReflectShaderModule &module, std::vector<VulDescriptorSetLayoutBindInfo> &bindInfos);
    void computePushConstant(const SpvReflectShaderModule &module, VkShaderStageFlags stage);

private:
    std::shared_ptr<VulLogicDevice> m_pLogicDevice;
    std::vector<VkPipelineShaderStageCreateInfo> m_shaderStages;
    VulPipelineVertexInputState m_pipelineVertexInputState;
    std::shared_ptr<VulDescriptorSetLayout> m_pDescriptorSetLayout;
    std::unique_ptr<VulShaderModule> m_pVertexShaderModule;
    std::unique_ptr<VulShaderModule> m_pFragShaderModule;
    std::vector<VkPushConstantRange> m_pushConstantRanges;
};
#endif //VULSHADER_H
