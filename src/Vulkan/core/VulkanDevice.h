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

using namespace HyperGpu;

class VulLogicDevice;
class VulSurface;
class VulInstance;
class VulPhysicalDevice;

class VulkanDevice final : public GpuDevice {
public:
	explicit						  VulkanDevice(const DeviceCreateInfo& info);
	~								  VulkanDevice() override;
	[[nodiscard]] PipelineManager*	  GetPipelineManager() override { return m_pPipelineManager; }
	[[nodiscard]] VulPhysicalDevice*  GetPhysicalDevice() const { return m_pPhysicalDevice; }
	[[nodiscard]] VulLogicDevice*	  GetLogicDevice() const { return m_pLogicDevice; }
	[[nodiscard]] VulSurface*		  GetVulSurface() const { return m_pSurface; }
	[[nodiscard]] GpuCmdManager*	  GetCmdManager() override { return m_pCmdManager; };
	[[nodiscard]] GpuResourceManager* GetResourceManager() override { return m_pResourceManager; };
	[[nodiscard]] GpuSurface*		  GetSurface(Pipeline* pipeline) override;
	[[nodiscard]] GpuSyncManager*	  GetSyncManager() override { return m_pSyncManager; };
	void							  Submit(GpuCmd* cmd, Semaphore* waitSemaphore, Semaphore* signalSemaphore, Fence* inFlightFence) override;
	void							  Present(Semaphore* waitSemaphore, GpuSurface* surface, uint32_t& imageIndex) override;

private:
	VulInstance*		m_pInstance		   = nullptr;
	VulSurface*			m_pSurface		   = nullptr;
	VulPhysicalDevice*	m_pPhysicalDevice  = nullptr;
	VulLogicDevice*		m_pLogicDevice	   = nullptr;
	GpuCmdManager*		m_pCmdManager	   = nullptr;
	PipelineManager*	m_pPipelineManager = nullptr;
	GpuResourceManager* m_pResourceManager = nullptr;
	GpuSyncManager*		m_pSyncManager	   = nullptr;
};

#endif // VULKANDEVICE_H
