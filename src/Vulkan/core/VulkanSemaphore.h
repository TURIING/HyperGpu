/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/16 16:45
 * @version: 1.0
 * @description:
 ********************************************************************************/
#ifndef VULKANSEMAPHORE_H
#define VULKANSEMAPHORE_H

#include <GpuSync.h>

#include "../../common/common.h"

USING_GPU_NAMESPACE_BEGIN

class VulSemaphore;
class VulkanDevice;

class VulkanSemaphore final : public Semaphore {
public:
	explicit VulkanSemaphore(VulkanDevice* device);
	~VulkanSemaphore() override;
	NODISCARD VulSemaphore* GetHandle() const { return m_pSemaphore; }

private:
	VulSemaphore* m_pSemaphore = nullptr;
};

USING_GPU_NAMESPACE_END
#endif // VULKANSEMAPHORE_H
