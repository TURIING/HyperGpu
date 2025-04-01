/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/22 13:14
 * @version: 1.0
 * @description:
 ********************************************************************************/
#include "VulkanBuffer.h"

#include "../base/resource/VulIndexBuffer.h"
#include "../base/resource/VulVertexBuffer.h"
#include "VulkanDevice.h"

VulkanBuffer::VulkanBuffer(VulkanDevice* device, BufferType type, const uint8_t* data, uint64_t size) : m_type(type) {
	switch(m_type) {
	case BufferType::Vertex:
		m_pBuffer = new VulVertexBuffer(device->GetLogicDevice(), data, size);
		break;
	case BufferType::Index:
		m_pBuffer = new VulIndexBuffer(device->GetLogicDevice(), data, size);
		break;
	default:
		LOG_ASSERT(false);
	}
}
