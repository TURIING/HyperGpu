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

class VulBuffer;
class VulLogicDevice;

class VulUniformBuffer final : public GpuObject {
public:
    VulUniformBuffer(VulLogicDevice* device, uint64_t bufferSize, uint32_t binding);
    ~VulUniformBuffer() override;
    [[nodiscard]] VkBuffer GetHandle() const;
    [[nodiscard]] uint32_t GetBinding() const { return m_binding; }
    [[nodiscard]] VkDescriptorBufferInfo GetDescriptorBufferInfo() const { return m_descriptorInfo; }
    void                   UpdateData(const uint8_t* data, uint64_t size) const;

private:
    VulBuffer* m_pBuffer = nullptr;
    uint32_t   m_binding = 0;
    VkDescriptorBufferInfo m_descriptorInfo {};
};

#endif //VULUNIFORMBUFFER_H
