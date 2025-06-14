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
class VulIndexBuffer;
class VulVertexBuffer;
class VulkanDevice;

class VulkanInputAssembler final: public InputAssembler {
public:
    VulkanInputAssembler(VulkanDevice * pDevice, const InputAssemblerInfo &info);
    ~VulkanInputAssembler() override;
    NODISCARD VulVertexBuffer* GetVertexBuffer() const { return m_pVertexBuffer; }
    NODISCARD VulIndexBuffer* GetIndexBuffer() const {  return m_pIndexBuffer; }
    NODISCARD u32 GetVertexCount() const { return m_vertexCount; }
    NODISCARD u32 GetIndexCount() const { return m_indexCount; }

private:
    VulkanDevice* m_pVulkanDevice = nullptr;
    VulVertexBuffer* m_pVertexBuffer = nullptr;
    VulIndexBuffer* m_pIndexBuffer = nullptr;
    u32 m_indexCount = 0;
    u32 m_vertexCount = 0;
};


USING_GPU_NAMESPACE_END
#endif //VULKANINPUTASSEMBLER_H
