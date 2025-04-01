/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/10 20:17
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULBUFFER_H
#define VULBUFFER_H

#include "../VulObject.h"
#include "../../../common/common.h"

class VulLogicDevice;
class VulBuffer;

struct VulBufferCreateInfo {
    VkDeviceSize size;
    VkBufferUsageFlags usage;
    VkMemoryPropertyFlags properties;
};

class VulBufferBuilder final{
public:
	VulBufferBuilder& SetLogicDevice(VulLogicDevice* device) {
		m_pLogicDevice = device;
		return *this;
	};

	VulBufferBuilder& SetSize(const VkDeviceSize size) {
		m_createInfo.size = size;
		return *this;
	};

	VulBufferBuilder& SetUsage(const VkBufferUsageFlags usage) {
		m_createInfo.usage = usage;
		return *this;
	};

	VulBufferBuilder& SetProperties(const VkMemoryPropertyFlags properties) {
		m_createInfo.properties = properties;
		return *this;
	};

	[[nodiscard]] VulBuffer* Build() const;

private:
	VulBufferCreateInfo m_createInfo{};
	VulLogicDevice*		m_pLogicDevice = nullptr;
};

class VulBuffer final: public VulObject<VkBuffer>{
public:
	 VulBuffer(VulLogicDevice* device, const VulBufferCreateInfo& info);
	~VulBuffer() override;

	[[nodiscard]] static VulBufferBuilder Builder() { return VulBufferBuilder{}; }

	[[nodiscard]] uint64_t GetSize() const { return m_size; }

	void MapData(uint64_t offset, uint64_t size, const void* data) const;
	void CopyBuffer(VulBuffer* dstBuffer, VkDeviceSize size = 0) const;
	void CopyFrom(VulBuffer* srcBuffer, VkDeviceSize size = 0) const;

private:
	VulLogicDevice* m_pLogicDevice	= nullptr;
	VkDeviceMemory	m_pDeviceMemory = nullptr;
	VkDeviceSize	m_size			= 0;
};

#endif // VULBUFFER_H
