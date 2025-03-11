/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/10 21:33
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulIndexBuffer.h"

#include "VulBuffer.h"

VulIndexBuffer::VulIndexBuffer(const std::shared_ptr<VulLogicDevice>& device, const std::vector<uint8_t>& bufferVertex): m_pLogicDevice(device){
    auto stageBuffer = VulBuffer::Builder()
                                    .SetLogicDevice(m_pLogicDevice)
                                    .SetSize(bufferVertex.size())
                                    .SetUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT)
                                    .SetProperties(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
                                    .Build();
    stageBuffer->MapData(0, bufferVertex.size(), bufferVertex.data());

    m_pBuffer = VulBuffer::Builder()
                    .SetLogicDevice(m_pLogicDevice)
                    .SetSize(bufferVertex.size())
                    .SetUsage(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT)
                    .SetProperties(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
                    .Build();
    // TODO: COPY BUFFER
    LOG_INFO("Vertex buffer created.");
}

VkBuffer VulIndexBuffer::GetHandle() const {
    return m_pBuffer->GetHandle();
}
