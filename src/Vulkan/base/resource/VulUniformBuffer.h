/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/4/2 21:45
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULUNIFORMBUFFER_H
#define VULUNIFORMBUFFER_H

#include "../../../common/common.h"

USING_GPU_NAMESPACE_BEGIN

class VulBuffer;
class VulLogicDevice;

class VulUniformBuffer final : public GpuObject {
public:
    VulUniformBuffer(VulLogicDevice* device, uint64_t bufferSize);
    ~VulUniformBuffer() override;
    [[nodiscard]] VkBuffer GetHandle() const;
    [[nodiscard]] VkDescriptorBufferInfo* GetDescriptorBufferInfo() { return &m_descriptorInfo; }
    void UpdateData(const void* data, uint64_t size) const;

private:
    VulBuffer* m_pBuffer = nullptr;
    VkDescriptorBufferInfo m_descriptorInfo {};
};

USING_GPU_NAMESPACE_END
#endif //VULUNIFORMBUFFER_H
