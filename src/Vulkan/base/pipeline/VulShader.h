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

class VulShader final: public GpuObject {
public:
    VulShader(VulLogicDevice* logicDevice, const ShaderInfo &shaderInfo);
    ~VulShader() override;
    NODISCARD std::vector<VkPipelineShaderStageCreateInfo> GetShaderStages() const { return m_shaderStages; }
    NODISCARD VulPipelineVertexInputState GetPipelineVertexInputState() { return m_pipelineVertexInputState; }
    NODISCARD VulDescriptorSetLayout* GetDescriptorSetLayout() const { return m_pDescriptorSetLayout; }
    NODISCARD std::vector<VkPushConstantRange> GetPushConstantRanges() const { return m_pushConstantRanges; }
    NODISCARD VulPipeLineLayout* GetPipelineLayout() const;
    NODISCARD std::unordered_map<std::string, uint8_t> GetResourceBinding() const { return m_mapResourceBinding; }

private:
    NODISCARD static VkPipelineShaderStageCreateInfo makeShaderStage(const SpvReflectShaderModule &module, VkShaderModule shaderModule);
    NODISCARD static VkShaderStageFlagBits transformSpvStageToVk(int spvStage);
    NODISCARD static uint32_t transformVkFormatToSize(VkFormat format);
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
    VulShaderModule* m_pCompShaderModule = nullptr;
    std::vector<VkPushConstantRange> m_pushConstantRanges;
    std::unordered_map<std::string, uint8_t> m_mapResourceBinding;
};

USING_GPU_NAMESPACE_END
#endif //VULSHADER_H
