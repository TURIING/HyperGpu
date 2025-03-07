/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/3 20:45
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULPIPELINELAYOUT_H
#define VULPIPELINELAYOUT_H

#include "VulObject.h"
#include "../../common/common.h"

class VulDescriptorSetLayout;
class VulLogicDevice;
class VulPipeLineLayout;

class VulPipelineLayoutBuilder {
public:
    VulPipelineLayoutBuilder();
    VulPipelineLayoutBuilder& SetLogicDevice(const std::shared_ptr<VulLogicDevice>& device) { m_pDevice = device; return *this; }
    VulPipelineLayoutBuilder& SetPushConstantRanges(const std::vector<VkPushConstantRange>& pushConstants);
    VulPipelineLayoutBuilder& SetDescriptorSetLayouts(const std::vector<VkDescriptorSetLayout>& descriptorLayouts);
    std::shared_ptr<VulPipeLineLayout> Build();

private:
    std::shared_ptr<VulLogicDevice> m_pDevice;
    VkPipelineLayoutCreateInfo m_pipelineLayoutCreateInfo{};
};

class VulPipeLineLayout: public VulObject<VkPipelineLayout>{
public:
    VulPipeLineLayout(const std::shared_ptr<VulLogicDevice> &device, const VkPipelineLayoutCreateInfo &pipelineLayoutCreateInfo);
    ~VulPipeLineLayout() override;
    [[nodiscard]] static VulPipelineLayoutBuilder Builder() { return VulPipelineLayoutBuilder{}; }

private:
    std::shared_ptr<VulLogicDevice> m_pDevice;
};

#endif //VULPIPELINELAYOUT_H
