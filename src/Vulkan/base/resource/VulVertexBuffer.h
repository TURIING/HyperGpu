/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/10 21:06
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULVERTEXBUFFER_H
#define VULVERTEXBUFFER_H

#include "../../../common/common.h"

USING_GPU_NAMESPACE_BEGIN

class VulBuffer;
class VulLogicDevice;

class VulVertexBuffer final : public GpuObject {
public:
	VulVertexBuffer(VulLogicDevice* device, const void* data, uint64_t size);
	~VulVertexBuffer() override;
	[[nodiscard]] VkBuffer GetHandle() const;

private:
	VulBuffer* m_pBuffer = nullptr;
};

USING_GPU_NAMESPACE_END
#endif //VULVERTEXBUFFER_H