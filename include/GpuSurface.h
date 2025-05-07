/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/16 15:13
 * @version: 1.0
 * @description:
 ********************************************************************************/
#ifndef GPUSURFACE_H
#define GPUSURFACE_H

namespace HyperGpu {
class Semaphore;

class GpuSurface : public GpuObject {
public:
	virtual Semaphore* AcquireNextImage(uint32_t& imageIndex) = 0;
};
}
#endif // GPUSURFACE_H
