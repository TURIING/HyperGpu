/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/2/22 00:34
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef OPENGLDEVICE_H
#define OPENGLDEVICE_H

#include "../../common/common.h"
#include "GpuDevice.h"

using namespace HyperGpu;

class OpenGlDevice final : public GpuDevice {
public:
    OpenGlDevice(const DeviceCreateInfo &info);
	[[nodiscard]] PipelineManager*	  GetPipelineManager() override;
	[[nodiscard]] GpuCmdManager*	  GetCmdManager() override;
	[[nodiscard]] GpuResourceManager* GetResourceManager() override;
	[[nodiscard]] GpuSyncManager*	  GetSyncManager() override;
	[[nodiscard]] GpuSurface*		  GetSurface(Pipeline* pipeline) override;
	void							  Submit(GpuCmd* cmd, Semaphore* waitSemaphore, Semaphore* signalSemaphore, Fence* inFlightFence) override;
	void							  Present(Semaphore* waitSemaphore, GpuSurface* surface, uint32_t& imageIndex) override;
};
#endif //OPENGLDEVICE_H
