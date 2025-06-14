/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/22 13:14
 * @version: 1.0
 * @description:
 ********************************************************************************/
#ifndef VULKANBUFFER_H
#define VULKANBUFFER_H

#include "../../../common/common.h"
#include "GpuResource.h"
USING_GPU_NAMESPACE_BEGIN

class VulUniformBuffer;
class VulIndexBuffer;
class VulVertexBuffer;
class VulkanDevice;

class VulkanBuffer final : public HyperGpu::Buffer {
public:
	VulkanBuffer(VulkanDevice* device, const BufferCreateInfo &createInfo);
	[[nodiscard]] VulVertexBuffer*  GetVertexBuffer() const { return std::get<VulVertexBuffer*>(m_pBuffer); }
	[[nodiscard]] VulIndexBuffer*   GetIndexBuffer() const { return std::get<VulIndexBuffer*>(m_pBuffer); }
	[[nodiscard]] VulUniformBuffer* GetUniformBuffer() const { return std::get<VulUniformBuffer*>(m_pBuffer); }
	void UpdateData(const uint8_t* data, uint64_t dataSize) override;

private:
	std::variant<VulVertexBuffer*, VulIndexBuffer*, VulUniformBuffer*> m_pBuffer;
	BufferType m_type;
};

USING_GPU_NAMESPACE_END
#endif // VULKANBUFFER_H
