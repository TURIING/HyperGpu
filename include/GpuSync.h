/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/16 16:33
 * @version: 1.0
 * @description:
 ********************************************************************************/
#ifndef GPUSYNC_H
#define GPUSYNC_H

#include "Gpu.h"
#include "Types.h"

namespace HyperGpu {
class Fence : public GpuObject {
public:
	virtual WaitState Wait(uint32_t timeout = 10000000) = 0;
	virtual void Reset() = 0;
};

class Semaphore : public GpuObject {};

class GpuSyncManager : public GpuObject {
public:
	[[nodiscard]] virtual Fence*	 CreateFence()	   = 0;
	[[nodiscard]] virtual Semaphore* CreateSemaphore() = 0;
	static void						 DestroyFence(Fence* fence) { fence->SubRef(); }
	static void						 DestroySemaphore(Semaphore* semaphore) { semaphore->SubRef(); }
};
}
#endif // GPUSYNC_H
