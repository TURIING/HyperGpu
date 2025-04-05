/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/29 21:22
 * @version: 1.0
 * @description:
 ********************************************************************************/
#ifndef GPU_H
#define GPU_H

#include <cassert>
#include <string>
#include <vector>
#include <functional>

class GpuObject {
public:
	virtual ~GpuObject() = default;
	void	 AddRef() { m_refCount.fetch_add(1, std::memory_order_relaxed); }
	void	 SubRef() {
		assert(m_refCount.load(std::memory_order_relaxed) >= 1);
		m_refCount.fetch_sub(1, std::memory_order_relaxed);
		if(m_refCount.load() == 0) {
			delete this;
		}
	}

private:
	std::atomic<uint32_t> m_refCount = 1;
};

#endif // GPU_H
