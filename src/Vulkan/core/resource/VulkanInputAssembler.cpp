/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/24 14:48
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulkanInputAssembler.h"

#include "../VulkanDevice.h"
#include "VulkanBuffer.h"

USING_GPU_NAMESPACE_BEGIN
    VulkanInputAssembler::VulkanInputAssembler(VulkanDevice* pDevice, const InputAssemblerInfo& info): m_pVulkanDevice(pDevice) {
    LOG_ASSERT(info.vertexCount > 0 && info.vertexSize > 0);
    m_pVulkanDevice->AddRef();

    Buffer::BufferCreateInfo vertexBufferInfo = {
        .bufferType = Buffer::Vertex,
        .bufferSize = info.vertexSize,
        .data = info.pVertexData
    };
    m_pVertexBuffer = new VulkanBuffer(m_pVulkanDevice, vertexBufferInfo);
    m_vertexCount = info.vertexCount;

    if (info.indexCount > 0) {
        m_indexCount = info.indexCount;
        Buffer::BufferCreateInfo indexBufferInfo = {
            .bufferType = Buffer::Index,
            .bufferSize = info.indexSize,
            .data = info.pIndexData
        };
        m_pIndexBuffer = new VulkanBuffer(m_pVulkanDevice, indexBufferInfo);
    }
}

VulkanInputAssembler::VulkanInputAssembler(VulkanDevice *pDevice, const InstanceInputAssemblerInfo &info): m_pVulkanDevice(pDevice) {
    LOG_ASSERT(info.vertexCount > 0 && info.vertexSize > 0);
    LOG_ASSERT(info.instanceAttributeCount > 0 && info.instanceDataSize > 0 && info.pInstanceData && info.instanceCount > 0);

    m_pVulkanDevice->AddRef();

    Buffer::BufferCreateInfo vertexBufferInfo = {
        .bufferType = Buffer::Vertex,
        .bufferSize = info.vertexSize,
        .data = info.pVertexData
    };
    m_pVertexBuffer = new VulkanBuffer(m_pVulkanDevice, vertexBufferInfo);
    m_vertexCount = info.vertexCount;

    if (info.indexCount > 0) {
        m_indexCount = info.indexCount;
        Buffer::BufferCreateInfo indexBufferInfo = {
            .bufferType = Buffer::Index,
            .bufferSize = info.indexSize,
            .data = info.pIndexData
        };
        m_pIndexBuffer = new VulkanBuffer(m_pVulkanDevice, indexBufferInfo);
    }

    Buffer::BufferCreateInfo instanceBufferInfo = {
        .bufferType = Buffer::Vertex,
        .bufferSize = info.instanceDataSize,
        .data = info.pInstanceData
    };
    m_pInstanceBuffer = new VulkanBuffer(m_pVulkanDevice, instanceBufferInfo);
    m_instanceCount = info.instanceCount;
}

VulkanInputAssembler::~VulkanInputAssembler() {
    m_pVulkanDevice->SubRef();
    m_pVertexBuffer->SubRef();
    if (m_pIndexBuffer) {
        m_pIndexBuffer->SubRef();
    }
}

USING_GPU_NAMESPACE_END
