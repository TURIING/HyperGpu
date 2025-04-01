/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/22 13:14
 * @version: 1.0
 * @description:
 ********************************************************************************/
#ifndef VULKANBUFFER_H
#define VULKANBUFFER_H

#include "../../common/common.h"
#include "GpuResource.h"

class VulIndexBuffer;
class VulVertexBuffer;
class VulkanDevice;

class VulkanBuffer final : public Buffer {
public:
	VulkanBuffer(VulkanDevice* device, BufferType type, const uint8_t* data, uint64_t size);

	[[nodiscard]] VulVertexBuffer* GetVertexBuffer() const { return std::get<VulVertexBuffer*>(m_pBuffer); }

	[[nodiscard]] VulIndexBuffer* GetIndexBuffer() const { return std::get<VulIndexBuffer*>(m_pBuffer); }

private:
	std::variant<VulVertexBuffer*, VulIndexBuffer*> m_pBuffer;
	BufferType										m_type;
};

#endif // VULKANBUFFER_H
