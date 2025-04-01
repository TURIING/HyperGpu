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

class VulkanDevice;
class VulCommandPool;

class VulkanCmdManager final : public GpuCmdManager{
public:
	explicit			  VulkanCmdManager(VulkanDevice* device);
	~					  VulkanCmdManager() override;
	[[nodiscard]] GpuCmd* CreateCommandBuffer() override;

private:
	VulkanDevice*	m_pVulkanDevice = nullptr;
	VulCommandPool* m_pCmdPool		= nullptr;
};

#endif //VULKANCMDMANAGER_H