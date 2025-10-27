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

class VulkanDevice;
class VulBuffer;

class VulkanBuffer final : public Buffer {
public:
	VulkanBuffer(VulkanDevice* device, const BufferCreateInfo &createInfo);
	~VulkanBuffer() override;
	void WriteData(const void* data, uint64_t dataSize) override;
	NODISCARD VkDescriptorBufferInfo* GetDescriptorBufferInfo();
	NODISCARD VkBuffer GetHandle() const;
	void Map(uint64_t offset, uint64_t size, void **pData) override;
	void UnMap() override;

private:
	void createVertexBuffer(const void *pData, uint64_t dataSize);
	void createIndexBuffer(const void *pData, uint64_t dataSize);
	void createUniformBuffer(const void *pData, uint64_t dataSize);
	void createTransferSrcBuffer(const void *pData, uint64_t dataSize);
	void createTransferDstBuffer(const void *pData, uint64_t dataSize);
	void createShaderStorageBuffer(const void *pData, uint64_t dataSize);

private:
	VulkanDevice* m_pVulkanDevice = nullptr;
	VulBuffer* m_pVulBuffer = nullptr;
	BufferType m_type;
    VkDescriptorBufferInfo m_descriptorInfo {};
};

USING_GPU_NAMESPACE_END
#endif // VULKANBUFFER_H
