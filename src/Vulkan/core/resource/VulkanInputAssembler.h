/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/24 14:48
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULKANINPUTASSEMBLER_H
#define VULKANINPUTASSEMBLER_H

#include "../../../common/common.h"

USING_GPU_NAMESPACE_BEGIN
class VulkanDevice;
class VulkanBuffer;

class VulkanInputAssembler final: public InputAssembler {
public:
    VulkanInputAssembler(VulkanDevice * pDevice, const InputAssemblerInfo &info);
    VulkanInputAssembler(VulkanDevice * pDevice, const InstanceInputAssemblerInfo &info);
    ~VulkanInputAssembler() override;
    NODISCARD VulkanBuffer* GetVertexBuffer() const { return m_pVertexBuffer; }
    NODISCARD VulkanBuffer* GetInstanceBuffer() const { return m_pInstanceBuffer; }
    NODISCARD VulkanBuffer* GetIndexBuffer() const {  return m_pIndexBuffer; }
    NODISCARD u32 GetVertexCount() const { return m_vertexCount; }
    NODISCARD u32 GetIndexCount() const { return m_indexCount; }
    NODISCARD u32 GetInstanceCount() const { return m_instanceCount; }

private:
    VulkanDevice* m_pVulkanDevice = nullptr;
    VulkanBuffer* m_pVertexBuffer = nullptr;
    VulkanBuffer* m_pInstanceBuffer = nullptr;
    VulkanBuffer* m_pIndexBuffer = nullptr;
    u32 m_indexCount = 0;
    u32 m_vertexCount = 0;
    u32 m_instanceCount = 1;
};


USING_GPU_NAMESPACE_END
#endif //VULKANINPUTASSEMBLER_H
