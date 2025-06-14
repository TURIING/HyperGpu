/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/10 21:33
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULINDEXBUFFER_H
#define VULINDEXBUFFER_H

#include "../../../common/common.h"

USING_GPU_NAMESPACE_BEGIN

class VulBuffer;
class VulLogicDevice;

class VulIndexBuffer final : public GpuObject {
public:
	VulIndexBuffer(VulLogicDevice* device, const uint8_t* data, uint64_t size);
	~VulIndexBuffer() override;
	[[nodiscard]] VkBuffer GetHandle() const;

private:
	VulBuffer* m_pBuffer = nullptr;
};

USING_GPU_NAMESPACE_END
#endif //VULINDEXBUFFER_H
