/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/2 14:14
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULDESCRIPTORPOOL_H
#define VULDESCRIPTORPOOL_H

#include "VulObject.h"
#include "../../common/common.h"
#include "VulTypeDefine.h"

class VulLogicDevice;
class VulDescriptorPool;

struct VulDescriptorPoolSizeInfo {
    VulDescriptorType type;
    uint32_t count;
};

struct VulDescriptorPoolCreateInfo {
    std::vector<VulDescriptorPoolSizeInfo> poolSizes;
};


class VulDescriptorPoolBuilder final {
public:
    explicit VulDescriptorPoolBuilder(const std::shared_ptr<VulLogicDevice>& device);
    VulDescriptorPoolBuilder& AddPoolSize(VulDescriptorPoolSizeInfo &&info);
    std::shared_ptr<VulDescriptorPool> Build();

private:
    VulDescriptorPoolCreateInfo m_createInfo {};
    std::shared_ptr<VulLogicDevice> m_pDevice;
};

class VulDescriptorPool final : public VulObject<VkDescriptorPool> {
public:
    VulDescriptorPool(const std::shared_ptr<VulLogicDevice>& device, const VulDescriptorPoolCreateInfo& info);
    ~VulDescriptorPool() override;

private:
    std::shared_ptr<VulLogicDevice> m_pDevice;
};

#endif //VULDESCRIPTORPOOL_H
