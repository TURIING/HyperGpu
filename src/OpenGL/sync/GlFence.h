/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/10 15:21
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef HYPERRENDER_GLFENCE_H
#define HYPERRENDER_GLFENCE_H

#include "../../common/common.h"

USING_GPU_NAMESPACE_BEGIN

class GlFence final : public Fence {
public:
    void Wait() const override;
    void Reset() const override;
    void Signal();

private:
    std::mutex m_lock;
    std::condition_variable m_condition;
    std::atomic<uint32_t> m_state;
};

USING_GPU_NAMESPACE_END
#endif //HYPERRENDER_GLFENCE_H