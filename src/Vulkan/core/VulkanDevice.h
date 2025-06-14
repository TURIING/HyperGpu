/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/2/22 00:09
 * @version: 1.0
 * @description:
 ********************************************************************************/
#ifndef VULKANDEVICE_H
#define VULKANDEVICE_H

#include "../../common/common.h"
#include "GpuDevice.h"

USING_GPU_NAMESPACE_BEGIN

class ResourceCache;
class VulLogicDevice;
class VulSurface;
class VulInstance;
class VulPhysicalDevice;

class VulkanDevice final : public GpuDevice {
public:
	explicit						  VulkanDevice(const DeviceCreateInfo& info);
	~								  VulkanDevice() override;
	[[nodiscard]] PipelineManager*    GetPipelineManager() override { return m_pPipelineManager; }
	[[nodiscard]] VulPhysicalDevice*  GetPhysicalDevice() const { return m_pPhysicalDevice; }
	[[nodiscard]] VulLogicDevice*     GetLogicDevice() const { return m_pLogicDevice; }
	[[nodiscard]] VulInstance*        GetInstance() const { return m_pInstance; }
	[[nodiscard]] GpuCmdManager*      GetCmdManager() override { return m_pCmdManager; };
	[[nodiscard]] GpuResourceManager* GetResourceManager() override { return m_pResourceManager; };
	[[nodiscard]] GpuSyncManager*     GetSyncManager() override { return m_pSyncManager; }
	[[nodiscard]] GpuSurface*         CreateSurface(const PlatformWindowInfo &platformWindowInfo) override;
	[[nodiscard]] Queue*			  CreateQueue(QueueType queueType) override;
	[[nodiscard]] ResourceCache*	  GetResourceCache() const { return m_pResourceCache; }

private:
	VulInstance*		m_pInstance		   = nullptr;
	VulPhysicalDevice*	m_pPhysicalDevice  = nullptr;
	VulLogicDevice*		m_pLogicDevice	   = nullptr;
	GpuCmdManager*		m_pCmdManager	   = nullptr;
	PipelineManager*	m_pPipelineManager = nullptr;
	GpuResourceManager* m_pResourceManager = nullptr;
	GpuSyncManager*		m_pSyncManager	   = nullptr;
	ResourceCache*		m_pResourceCache   = nullptr;
};

USING_GPU_NAMESPACE_END
#endif // VULKANDEVICE_H
