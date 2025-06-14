/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 17:16
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULSHADER_H
#define VULSHADER_H

#include "../../../common/common.h"
#include "../VulTypeDefine.h"

struct SpvReflectShaderModule;
USING_GPU_NAMESPACE_BEGIN

class VulPipeLineLayout;
class VulShaderModule;
struct VulDescriptorSetLayoutBindInfo;
class VulDescriptorSetLayout;
class VulLogicDevice;

class VulShader final {
public:
    VulShader(VulLogicDevice* logicDevice, const ShaderInfo &shaderInfo);
    ~VulShader();
    [[nodiscard]] std::vector<VkPipelineShaderStageCreateInfo> GetShaderStages() const { return m_shaderStages; }
    [[nodiscard]] VulPipelineVertexInputState GetPipelineVertexInputState() { return m_pipelineVertexInputState; }
    [[nodiscard]] VulDescriptorSetLayout* GetDescriptorSetLayout() const { return m_pDescriptorSetLayout; }
    [[nodiscard]] std::vector<VkPushConstantRange> GetPushConstantRanges() const { return m_pushConstantRanges; }
    [[nodiscard]] VulPipeLineLayout* GetPipelineLayout() const;
    [[nodiscard]] std::unordered_map<std::string, uint8_t> GetResourceBinding() const { return m_mapResourceBinding; }

private:
    [[nodiscard]] static VkPipelineShaderStageCreateInfo makeShaderStage(const SpvReflectShaderModule &module, VkShaderModule shaderModule);
    [[nodiscard]] static VkShaderStageFlagBits transformSpvStageToVk(int spvStage);
    [[nodiscard]] static uint32_t transformVkFormatToSize(VkFormat format);
    void computeInputState(const SpvReflectShaderModule &module);
    void computeDescriptorSetLayout(const SpvReflectShaderModule &module, std::vector<VulDescriptorSetLayoutBindInfo> &bindInfos);
    void computePushConstant(const SpvReflectShaderModule &module, VkShaderStageFlags stage);

private:
    VulLogicDevice* m_pLogicDevice = nullptr;
    std::vector<VkPipelineShaderStageCreateInfo> m_shaderStages;
    VulPipelineVertexInputState m_pipelineVertexInputState;
    VulDescriptorSetLayout* m_pDescriptorSetLayout = nullptr;
    VulShaderModule* m_pVertexShaderModule = nullptr;
    VulShaderModule* m_pFragShaderModule = nullptr;
    std::vector<VkPushConstantRange> m_pushConstantRanges;
    std::unordered_map<std::string, uint8_t> m_mapResourceBinding;
};

USING_GPU_NAMESPACE_END
#endif //VULSHADER_H
