/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/9 20:55
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULKANCMDMANAGER_H
#define VULKANCMDMANAGER_H

#include <GpuCmd.h>

#include "../../common/common.h"

USING_GPU_NAMESPACE_BEGIN

class VulQueue;
class VulkanDevice;
class VulCommandPool;
class VulCommandBuffer;

class VulkanCmdManager final : public GpuCmdManager{
public:
	explicit VulkanCmdManager(VulkanDevice* device);
	~VulkanCmdManager() override;
	[[nodiscard]] GpuCmd* CreateCommandBuffer() override;
	void WithSingleCmdBuffer(const std::function<void(VulCommandBuffer* cmd)>& func) const;
	void WithSingleCmdBuffer(const std::function<void(GpuCmd*)>& func) override;

private:
	VulkanDevice*	m_pVulkanDevice = nullptr;
	VulCommandPool* m_pCmdPool		= nullptr;
	VulQueue*		m_pQueue		= nullptr;
};

USING_GPU_NAMESPACE_END
#endif //VULKANCMDMANAGER_H