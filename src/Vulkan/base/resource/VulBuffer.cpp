/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/10 20:17
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulBuffer.h"

#include "../VulLogicDevice.h"
#include "../VulPhysicalDevice.h"

VulBuffer::VulBuffer(const std::shared_ptr<VulLogicDevice>& device, const VulBufferCreateInfo& info): m_pLogicDevice(device) {
    // 创建缓冲
    VkBufferCreateInfo bufferInfo {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = info.size,
        .usage = info.usage,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE
    };
    CALL_VK(vkCreateBuffer(m_pLogicDevice->GetHandle(), &bufferInfo, nullptr, &m_pHandle));

    // 获取缓冲的内存需求
    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(m_pLogicDevice->GetHandle(), m_pHandle, &memoryRequirements);

    // 分配内存
    const VkMemoryAllocateInfo allocateInfo {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = memoryRequirements.size,
        .memoryTypeIndex = m_pLogicDevice->GetPhysicalDevice()->FindMemoryType(memoryRequirements.memoryTypeBits, info.properties)
    };
    CALL_VK(vkAllocateMemory(m_pLogicDevice->GetHandle(), &allocateInfo, nullptr, &m_pDeviceMemory));

    // 将缓冲区绑定到设备内存
    vkBindBufferMemory(m_pLogicDevice->GetHandle(), m_pHandle, m_pDeviceMemory, 0);
}

VulBuffer::~VulBuffer() {
    vkDestroyBuffer(m_pLogicDevice->GetHandle(), m_pHandle, nullptr);
    vkFreeMemory(m_pLogicDevice->GetHandle(), m_pDeviceMemory, nullptr);
}

void VulBuffer::MapData(uint64_t offset, uint64_t size, const void* data) const {
    void *tempPtr = nullptr;
    vkMapMemory(m_pLogicDevice->GetHandle(), m_pDeviceMemory, offset, size, 0, &tempPtr);
    memcpy(tempPtr, data, size);
    vkUnmapMemory(m_pLogicDevice->GetHandle(), m_pDeviceMemory);
}

std::unique_ptr<VulBuffer> VulBufferBuilder::Build() {
    return std::make_unique<VulBuffer>(m_pLogicDevice, m_createInfo);
}
