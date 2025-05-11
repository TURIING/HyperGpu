/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/10 15:20
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef HYPERRENDER_GLSEMAPHORE_H
#define HYPERRENDER_GLSEMAPHORE_H

#include "../../common/common.h"

USING_GPU_NAMESPACE_BEGIN

class GlSemaphore final : public Semaphore {
public:
    WaitState Wait(u32 timeout);
    void Signal();

private:
    std::mutex m_lock;
    std::condition_variable m_condition;
    std::atomic<uint32_t> m_count = 0;
};

USING_GPU_NAMESPACE_END
#endif //HYPERRENDER_GLSEMAPHORE_H
