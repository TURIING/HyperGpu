/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/10 20:17
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULBUFFER_H
#define VULBUFFER_H

#include "../VulObject.h"
#include "../../../common/common.h"

class VulLogicDevice;
class VulBuffer;

struct VulBufferCreateInfo {
    VkDeviceSize size;
    VkBufferUsageFlags usage;
    VkMemoryPropertyFlags properties;
};

class VulBufferBuilder final{
public:
    VulBufferBuilder& SetLogicDevice(const std::shared_ptr<VulLogicDevice>& device) { m_pLogicDevice = device; return *this; };
    VulBufferBuilder& SetSize(const VkDeviceSize size) { m_createInfo.size = size; return *this; };
    VulBufferBuilder& SetUsage(const VkBufferUsageFlags usage) { m_createInfo.usage = usage; return *this; };
    VulBufferBuilder& SetProperties(const VkMemoryPropertyFlags properties) { m_createInfo.properties = properties; return *this; };
    [[nodiscard]] std::unique_ptr<VulBuffer> Build();

private:
    VulBufferCreateInfo m_createInfo {};
    std::shared_ptr<VulLogicDevice> m_pLogicDevice;
};

class VulBuffer final: public VulObject<VkBuffer>{
public:
    VulBuffer(const std::shared_ptr<VulLogicDevice>& device, const VulBufferCreateInfo& info);
    ~VulBuffer() override;
    [[nodiscard]] static VulBufferBuilder Builder() { return VulBufferBuilder{}; }
    void MapData(uint64_t offset, uint64_t size, const void* data) const;

private:
    std::shared_ptr<VulLogicDevice> m_pLogicDevice;
    VkDeviceMemory m_pDeviceMemory = nullptr;
};



#endif //VULBUFFER_H
