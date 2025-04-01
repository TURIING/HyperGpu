/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/2/22 22:33
* @version: 1.0
* @description: 
********************************************************************************/

#include "OpenGlDevice.h"

OpenGlDevice::OpenGlDevice(const DeviceCreateInfo& info) {
}

PipelineManager* OpenGlDevice::GetPipelineManager() {
	return nullptr;
}

GpuCmdManager* OpenGlDevice::GetCmdManager() {
	return nullptr;
}

GpuResourceManager* OpenGlDevice::GetResourceManager() {
	return nullptr;
}

GpuSyncManager* OpenGlDevice::GetSyncManager() {
	return nullptr;
}

GpuSurface* OpenGlDevice::GetSurface(Pipeline* pipeline) {
	return nullptr;
}
void OpenGlDevice::Submit(GpuCmd* cmd, Semaphore* waitSemaphore, Semaphore* signalSemaphore, Fence* inFlightFence) {}
void OpenGlDevice::Present(Semaphore* waitSemaphore, GpuSurface* surface, uint32_t& imageIndex) {}
