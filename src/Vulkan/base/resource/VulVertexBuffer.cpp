/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/10 21:06
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulVertexBuffer.h"

#include "VulBuffer.h"

VulVertexBuffer::VulVertexBuffer(VulLogicDevice* device, const uint8_t* data, uint64_t size) {
	const auto stageBuffer = VulBuffer::Builder()
								 .SetLogicDevice(device)
								 .SetSize(size)
								 .SetUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT)
								 .SetProperties(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
								 .Build();
	stageBuffer->MapData(0, size, data);

	m_pBuffer = VulBuffer::Builder()
					.SetLogicDevice(device)
					.SetSize(size)
					.SetUsage(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT)
					.SetProperties(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
					.Build();
	m_pBuffer->CopyFrom(stageBuffer);

	stageBuffer->SubRef();
	LOG_INFO("Vertex buffer created, size: {}.", size);
}

VulVertexBuffer::~VulVertexBuffer() {
	m_pBuffer->SubRef();
}

VkBuffer VulVertexBuffer::GetHandle() const {
    return m_pBuffer->GetHandle();
}
