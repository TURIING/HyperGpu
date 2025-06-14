/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/22 13:14
 * @version: 1.0
 * @description:
 ********************************************************************************/
#include "VulkanBuffer.h"

#include "../../base/resource/VulIndexBuffer.h"
#include "../../base/resource/VulVertexBuffer.h"
#include "../VulkanDevice.h"
#include "../../base/resource/VulUniformBuffer.h"

USING_GPU_NAMESPACE_BEGIN

VulkanBuffer::VulkanBuffer(VulkanDevice* device, const BufferCreateInfo &createInfo) : m_type(createInfo.bufferType) {
	const auto [type, size, data] = createInfo;

	switch(m_type) {
	case BufferType::Vertex:
		m_pBuffer = new VulVertexBuffer(device->GetLogicDevice(), data, size);
		break;
	case BufferType::Index:
		m_pBuffer = new VulIndexBuffer(device->GetLogicDevice(), data, size);
		break;
	case BufferType::Uniform:
		m_pBuffer = new VulUniformBuffer(device->GetLogicDevice(), size);
		break;
	default:
		LOG_ASSERT(false);
	}
}

void VulkanBuffer::UpdateData(const uint8_t* data, uint64_t dataSize) {
	if(m_type == BufferType::Uniform) {
		std::get<VulUniformBuffer*>(m_pBuffer)->UpdateData(data, dataSize);
	} else {
		// todo: update data for vertex/index buffer
		LOG_ASSERT(false);
	}
}
USING_GPU_NAMESPACE_END