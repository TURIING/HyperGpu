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
#include "common/common.h"
#include "Vulkan/base/VulTypeDefine.h"

class VulLogicDevice;

struct VulDescriptorSetLayoutBindInfo {
    uint32_t bindIndex = 0;
    VulDescriptorType descriptorType = VulDescriptorType::Sampler;
};

struct VulDescriptorSetLayoutCreateInfo {
    std::vector<VulDescriptorSetLayoutBindInfo> descriptorSetLayoutBindings;
};

class VulDescriptorSetLayout final : public VulObject<VkDescriptorSetLayout>{
public:
    VulDescriptorSetLayout(const std::shared_ptr<VulLogicDevice> &device, const VulDescriptorSetLayoutCreateInfo &createInfo);
    ~VulDescriptorSetLayout() override;

private:
    std::shared_ptr<VulLogicDevice> m_pDevice;
};

#endif //VULDESCRIPTORLAYOUT_H
