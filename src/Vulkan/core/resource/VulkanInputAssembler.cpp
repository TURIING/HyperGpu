/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/24 14:48
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulkanInputAssembler.h"

#include "../VulkanDevice.h"
#include "../../base/resource/VulVertexBuffer.h"
#include "../../base/resource/VulIndexBuffer.h"

USING_GPU_NAMESPACE_BEGIN
VulkanInputAssembler::VulkanInputAssembler(VulkanDevice* pDevice, const InputAssemblerInfo& info): m_pVulkanDevice(pDevice) {
    LOG_ASSERT(info.vertexCount > 0 && info.vertexSize > 0);

    m_pVulkanDevice->AddRef();
    m_pVertexBuffer = new VulVertexBuffer(m_pVulkanDevice->GetLogicDevice(), info.pVertexData, info.vertexSize);
    m_vertexCount = info.vertexCount;

    if (info.indexCount > 0) {
        m_indexCount = info.indexCount;
        m_pIndexBuffer = new VulIndexBuffer(m_pVulkanDevice->GetLogicDevice(),  info.pIndexData, info.indexSize);
    }
}

VulkanInputAssembler::~VulkanInputAssembler() {
    m_pVulkanDevice->SubRef();
    m_pVertexBuffer->SubRef();
    if (m_pIndexBuffer) {
        m_pIndexBuffer->SubRef();
    }
}

USING_GPU_NAMESPACE_END
