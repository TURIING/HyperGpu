/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/16 16:45
 * @version: 1.0
 * @description:
 ********************************************************************************/
#ifndef VULKANFENCE_H
#define VULKANFENCE_H

#include "../../common/common.h"

USING_GPU_NAMESPACE_BEGIN

class VulFence;
class VulkanDevice;
using namespace HyperGpu;

class VulkanFence : public Fence {
public:
	explicit				VulkanFence(const VulkanDevice* device);
	~						VulkanFence() override;
	WaitState Wait(uint32_t timeout) override;
	void					Reset() override;
	[[nodiscard]] VulFence* GetHandle() const { return m_pFence; };

private:
	VulFence* m_pFence;
};

USING_GPU_NAMESPACE_END
#endif // VULKANFENCE_H
