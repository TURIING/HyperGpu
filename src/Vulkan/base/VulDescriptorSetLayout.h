/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/2 14:35
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULDESCRIPTORLAYOUT_H
#define VULDESCRIPTORLAYOUT_H

#include "VulObject.h"
#include "../../common/common.h"
#include "VulTypeDefine.h"

class VulLogicDevice;
class VulDescriptorSetLayout;

struct VulDescriptorSetLayoutBindInfo {
    uint32_t bindIndex = 0;
    VulDescriptorType descriptorType = VulDescriptorType::Sampler;
    VkShaderStageFlags stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    uint32_t descriptorCount = 0;
};

struct VulDescriptorSetLayoutCreateInfo {
    std::vector<VulDescriptorSetLayoutBindInfo> descriptorSetLayoutBindings;
};


class VulDescriptorSetLayoutBuilder final {
public:
    VulDescriptorSetLayoutBuilder& AddDescriptorBinding(VulDescriptorSetLayoutBindInfo &&bindInfo);
    VulDescriptorSetLayoutBuilder& AddDescriptorBindings(const std::vector<VulDescriptorSetLayoutBindInfo> &bindInfos);
    VulDescriptorSetLayoutBuilder& SetLogicDevice(const std::shared_ptr<VulLogicDevice>& device) { m_pDevice = device; return *this; };
    std::shared_ptr<VulDescriptorSetLayout> Build();

private:
    std::shared_ptr<VulLogicDevice> m_pDevice;
    VulDescriptorSetLayoutCreateInfo m_createInfo;
};

class VulDescriptorSetLayout final : public VulObject<VkDescriptorSetLayout>{
public:
    VulDescriptorSetLayout(const std::shared_ptr<VulLogicDevice> &device, const VulDescriptorSetLayoutCreateInfo &createInfo);
    ~VulDescriptorSetLayout() override;
    [[nodiscard]] static VulDescriptorSetLayoutBuilder Builder();

private:
    std::shared_ptr<VulLogicDevice> m_pDevice;
};

#endif //VULDESCRIPTORLAYOUT_H
