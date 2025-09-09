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
class VulDescriptorPool;

class VulkanDevice final : public GpuDevice {
public:
	explicit VulkanDevice(const DeviceCreateInfo& info);
	~VulkanDevice() override;
	NODISCARD PipelineManager*    GetPipelineManager() override { return m_pPipelineManager; }
	NODISCARD VulPhysicalDevice*  GetPhysicalDevice() const { return m_pPhysicalDevice; }
	NODISCARD VulLogicDevice*     GetLogicDevice() const { return m_pLogicDevice; }
	NODISCARD VulInstance*        GetInstance() const { return m_pInstance; }
	NODISCARD GpuCmdManager*      GetCmdManager() override { return m_pCmdManager; };
	NODISCARD GpuResourceManager* GetResourceManager() override { return m_pResourceManager; };
	NODISCARD GpuSyncManager*     GetSyncManager() override { return m_pSyncManager; }
	NODISCARD GpuSurface*         CreateSurface(const PlatformWindowInfo &platformWindowInfo) override;
	NODISCARD Queue*			  CreateQueue(QueueType queueType) override;
	NODISCARD ResourceCache*	  GetResourceCache() const { return m_pResourceCache; }
	NODISCARD VulDescriptorPool*  GetDescriptorPool() const { return m_pDescriptorPool; }

private:
	VulInstance*		m_pInstance		   = nullptr;
	VulPhysicalDevice*	m_pPhysicalDevice  = nullptr;
	VulLogicDevice*		m_pLogicDevice	   = nullptr;
	GpuCmdManager*		m_pCmdManager	   = nullptr;
	PipelineManager*	m_pPipelineManager = nullptr;
	GpuResourceManager* m_pResourceManager = nullptr;
	GpuSyncManager*		m_pSyncManager	   = nullptr;
	ResourceCache*		m_pResourceCache   = nullptr;
	VulDescriptorPool*  m_pDescriptorPool  = nullptr;
};

USING_GPU_NAMESPACE_END
#endif // VULKANDEVICE_H
