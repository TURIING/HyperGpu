/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/4/2 21:45
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulUniformBuffer.h"

#include "VulBuffer.h"

VulUniformBuffer::VulUniformBuffer(VulLogicDevice* device, uint64_t bufferSize, uint32_t binding): m_binding(binding) {
    m_pBuffer = VulBuffer::Builder()
                .SetLogicDevice(device)
                .SetSize(bufferSize)
                .SetUsage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
                .SetProperties(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
                .Build();

    m_descriptorInfo = {
        .buffer = m_pBuffer->GetHandle(),
        .offset = 0,
        .range = bufferSize
    };

    LOG_INFO("Uniform buffer created, binding: {}, size: {}.", m_binding, bufferSize);
}

VulUniformBuffer::~VulUniformBuffer() {
    m_pBuffer->SubRef();
}

VkBuffer VulUniformBuffer::GetHandle() const {
    return m_pBuffer->GetHandle();
}

void VulUniformBuffer::UpdateData(const uint8_t* data, uint64_t size) const {
    m_pBuffer->MapData(0, size, data);
}
