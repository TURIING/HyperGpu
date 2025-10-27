/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/22 13:14
 * @version: 1.0
 * @description:
 ********************************************************************************/
#include "VulkanBuffer.h"

#include "../../base/resource/VulBuffer.h"
#include "../VulkanDevice.h"
#include "GpuResource.h"

USING_GPU_NAMESPACE_BEGIN

VulkanBuffer::VulkanBuffer(VulkanDevice* device, const BufferCreateInfo &createInfo) : m_pVulkanDevice(device), m_type(createInfo.bufferType) {
	m_pVulkanDevice->AddRef();

	const auto [type, size, data] = createInfo;

	switch(m_type) {
	case BufferType::Vertex:
		createVertexBuffer(data, size);
		break;
	case BufferType::Index:
		createIndexBuffer(data, size);
		break;
	case BufferType::Uniform:
		createUniformBuffer(data, size);
		break;
	case BufferType::TransferSrc:
		createTransferSrcBuffer(data, size);
		break;
	case BufferType::TransferDst:
		createTransferDstBuffer(data, size);
		break;
	case BufferType::ShaderStorage:
		createShaderStorageBuffer(data, size);
		break;
	default:
		LOG_ASSERT(false);
	}
}

VulkanBuffer::~VulkanBuffer() {
	m_pVulBuffer->SubRef();
	m_pVulkanDevice->SubRef();
}

void VulkanBuffer::WriteData(const void* data, uint64_t dataSize) {
	if(m_type == BufferType::Uniform) {
		m_pVulBuffer->WriteData(0, dataSize, data);
	} else {
		// todo: update data for vertex/index buffer
		LOG_ASSERT(false);
	}
}

VkDescriptorBufferInfo * VulkanBuffer::GetDescriptorBufferInfo() {
	LOG_ASSERT_INFO(m_type == BufferType::Uniform, "UnSupported buffer type");
	return &m_descriptorInfo;
}

VkBuffer VulkanBuffer::GetHandle() const {
	return m_pVulBuffer->GetHandle();
}

void VulkanBuffer::Map(uint64_t offset, uint64_t size, void **pData) {
	m_pVulBuffer->Map(offset, size, pData);
}

void VulkanBuffer::UnMap() {
	m_pVulBuffer->Unmap();
}

void VulkanBuffer::createVertexBuffer(const void *pData, uint64_t dataSize) {
	const auto logicDevice = m_pVulkanDevice->GetLogicDevice();

	const auto stageBuffer = VulBuffer::Builder()
								 .SetLogicDevice(logicDevice)
								 .SetSize(dataSize)
								 .SetUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT)
								 .SetProperties(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
								 .Build();
	stageBuffer->WriteData(0, dataSize, pData);

	m_pVulBuffer = VulBuffer::Builder()
					.SetLogicDevice(logicDevice)
					.SetSize(dataSize)
					.SetUsage(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT)
					.SetProperties(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
					.Build();
	m_pVulBuffer->CopyFrom(stageBuffer);

	stageBuffer->SubRef();
}

void VulkanBuffer::createIndexBuffer(const void *pData, uint64_t dataSize) {
	const auto logicDevice = m_pVulkanDevice->GetLogicDevice();

	const auto stageBuffer = VulBuffer::Builder()
								 .SetLogicDevice(logicDevice)
								 .SetSize(dataSize)
								 .SetUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT)
								 .SetProperties(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
								 .Build();
	stageBuffer->WriteData(0, dataSize, pData);

	m_pVulBuffer = VulBuffer::Builder()
					.SetLogicDevice(logicDevice)
					.SetSize(dataSize)
					.SetUsage(VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT)
					.SetProperties(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT)
					.Build();
	m_pVulBuffer->CopyFrom(stageBuffer);

	stageBuffer->SubRef();
}

void VulkanBuffer::createUniformBuffer(const void *pData, uint64_t dataSize) {
	const auto logicDevice = m_pVulkanDevice->GetLogicDevice();

	m_pVulBuffer = VulBuffer::Builder()
				.SetLogicDevice(logicDevice)
				.SetSize(dataSize)
				.SetUsage(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
				.SetProperties(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
				.Build();

	if (pData) {
		m_pVulBuffer->WriteData(0, dataSize, pData);
	}

	m_descriptorInfo = {
		.buffer = m_pVulBuffer->GetHandle(),
		.offset = 0,
		.range = dataSize
	};
}

void VulkanBuffer::createTransferSrcBuffer(const void *pData, uint64_t dataSize) {
	const auto logicDevice = m_pVulkanDevice->GetLogicDevice();

	m_pVulBuffer = VulBuffer::Builder()
				.SetLogicDevice(logicDevice)
				.SetSize(dataSize)
				.SetUsage(VK_BUFFER_USAGE_TRANSFER_SRC_BIT)
				.SetProperties(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
				.Build();

	if (pData) {
		m_pVulBuffer->WriteData(0, dataSize, pData);
	}
}

void VulkanBuffer::createTransferDstBuffer(const void* pData, uint64_t dataSize) {
	const auto logicDevice = m_pVulkanDevice->GetLogicDevice();

	m_pVulBuffer = VulBuffer::Builder()
					   .SetLogicDevice(logicDevice)
					   .SetSize(dataSize)
					   .SetUsage(VK_BUFFER_USAGE_TRANSFER_DST_BIT)
					   .SetProperties(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
					   .Build();

	if(pData) {
		m_pVulBuffer->WriteData(0, dataSize, pData);
	}
}

void VulkanBuffer::createShaderStorageBuffer(const void* pData, uint64_t dataSize) {
	const auto logicDevice = m_pVulkanDevice->GetLogicDevice();

	m_pVulBuffer = VulBuffer::Builder()
				.SetLogicDevice(logicDevice)
				.SetSize(dataSize)
				.SetUsage(VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT)
				.SetProperties(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
				.Build();

	if (pData) {
		m_pVulBuffer->WriteData(0, dataSize, pData);
	}
}

USING_GPU_NAMESPACE_END
