/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/16 16:38
 * @version: 1.0
 * @description:
 ********************************************************************************/
#ifndef VULKANSYNCMANAGER_H
#define VULKANSYNCMANAGER_H

#include "VulkanDevice.h"

class VulkanSyncManager final : public GpuSyncManager {
public:
	explicit                 VulkanSyncManager(VulkanDevice* device);
	~                        VulkanSyncManager() override;
	[[nodiscard]] Fence*     CreateFence() override;
	[[nodiscard]] Semaphore* CreateSemaphore() override;

private:
	VulkanDevice* m_pVulkanDevice;
};

#endif // VULKANSYNCMANAGER_H
