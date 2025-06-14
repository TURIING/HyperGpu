/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/3 20:45
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULPIPELINELAYOUT_H
#define VULPIPELINELAYOUT_H

#include "../VulObject.h"
#include "../../../common/common.h"

USING_GPU_NAMESPACE_BEGIN

class VulDescriptorSetLayout;
class VulLogicDevice;
class VulPipeLineLayout;

class VulPipelineLayoutBuilder {
public:
    VulPipelineLayoutBuilder();
    VulPipelineLayoutBuilder& SetLogicDevice(VulLogicDevice* device) { m_pDevice = device; return *this; }
    VulPipelineLayoutBuilder& SetPushConstantRanges(const std::vector<VkPushConstantRange>& pushConstants);
    VulPipelineLayoutBuilder& SetDescriptorSetLayouts(const std::vector<VkDescriptorSetLayout>& descriptorLayouts);
    [[nodiscard]] VulPipeLineLayout* Build() const;

private:
    VulLogicDevice* m_pDevice = nullptr;
    VkPipelineLayoutCreateInfo m_pipelineLayoutCreateInfo{};
};

class VulPipeLineLayout: public VulObject<VkPipelineLayout> {
public:
    VulPipeLineLayout(VulLogicDevice* device, const VkPipelineLayoutCreateInfo &pipelineLayoutCreateInfo);
    ~VulPipeLineLayout() override;
    [[nodiscard]] static VulPipelineLayoutBuilder Builder() { return VulPipelineLayoutBuilder{}; }

private:
    VulLogicDevice* m_pDevice = nullptr;
};

USING_GPU_NAMESPACE_END
#endif //VULPIPELINELAYOUT_H
