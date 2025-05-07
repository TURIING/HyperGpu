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

namespace HyperGpu {
class Fence : public GpuObject {
public:
	virtual void Wait() const  = 0;
	virtual void Reset() const = 0;
};

class Semaphore : public GpuObject {
public:
	virtual ~Semaphore() = default;
};

class GpuSyncManager : public GpuObject {
public:
	virtual ~						 GpuSyncManager()  = default;
	[[nodiscard]] virtual Fence*	 CreateFence()	   = 0;
	[[nodiscard]] virtual Semaphore* CreateSemaphore() = 0;
	static void						 DestroyFence(Fence* fence) { fence->SubRef(); }
	static void						 DestroySemaphore(Semaphore* semaphore) { semaphore->SubRef(); }
};
}
#endif // GPUSYNC_H
