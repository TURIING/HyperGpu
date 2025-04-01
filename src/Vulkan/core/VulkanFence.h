/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/16 16:45
 * @version: 1.0
 * @description:
 ********************************************************************************/
#ifndef VULKANFENCE_H
#define VULKANFENCE_H

#include <GpuSync.h>

#include "../../common/common.h"
class VulFence;
class VulkanDevice;
using namespace HyperGpu;

class VulkanFence : public Fence {
public:
	explicit				VulkanFence(const VulkanDevice* device);
	~						VulkanFence() override;
	void					Wait() const override;
	void					Reset() const override;
	[[nodiscard]] VulFence* GetHandle() const { return m_pFence; };

private:
	VulFence* m_pFence;
};

#endif // VULKANFENCE_H
