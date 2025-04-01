/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/2/22 00:01
 * @version: 1.0
 * @description:
 ********************************************************************************/
#ifndef COREDEFINE_H
#define COREDEFINE_H

#include <GpuResource.h>

#include "GpuCmd.h"
#include "GpuPipeline.h"
#include "GpuSurface.h"
#include "GpuSync.h"
#include "Types.h"

namespace HyperGpu {
struct DeviceSupportInfo {};

class GpuDevice : public GpuObject {
public:
	~										  GpuDevice() override																			  = default;
	[[nodiscard]] virtual PipelineManager*	  GetPipelineManager()																			  = 0;
	[[nodiscard]] virtual GpuCmdManager*	  GetCmdManager()																				  = 0;
	[[nodiscard]] virtual GpuResourceManager* GetResourceManager()																			  = 0;
	[[nodiscard]] virtual GpuSyncManager*	  GetSyncManager()																				  = 0;
	[[nodiscard]] virtual GpuSurface*		  GetSurface(Pipeline* pipeline)																  = 0;
	virtual void							  Submit(GpuCmd* cmd, Semaphore* waitSemaphore, Semaphore* signalSemaphore, Fence* inFlightFence) = 0;
	virtual void							  Present(Semaphore* waitSemaphore, GpuSurface* surface, uint32_t& imageIndex)					  = 0;
};

struct PlatformWindowInfo {
	void* handle = nullptr;
	Size  size;
};

struct DeviceCreateInfo {
	PlatformWindowInfo platformWindowInfo;
};

class GpuFactory final : public GpuObject {
public:
	enum GpuType { OPENGL, VULKAN };

public:
	~						 GpuFactory() = default;
	explicit				 GpuFactory(GpuType type);
	[[nodiscard]] GpuDevice* CreateDevice(const DeviceCreateInfo& info) const;
	static void				 DestroyDevice(GpuDevice* device) { device->SubRef(); };

private:
	GpuType m_type;
};
} // namespace HyperGpu

#endif // COREDEFINE_H
