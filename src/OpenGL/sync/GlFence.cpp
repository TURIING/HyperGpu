/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/10 15:21
* @version: 1.0
* @description: 
********************************************************************************/
#include "GlFence.h"
USING_GPU_NAMESPACE_BEGIN

WaitState GlFence::Wait(u32 timeout) {
    std::unique_lock<std::mutex> locker(m_lock);
    if (m_state > 0) {
        return WaitState::Success;
    }

    if (timeout > 0) {
        while (true) {
            if (m_condition.wait_for(locker, std::chrono::nanoseconds(timeout), [&]{ return m_state > 0; })) {
                return WaitState::Success;
            }
            return WaitState::Timeout;
        }
    }
    else {
        m_condition.wait(locker, [&] { return m_state > 0; });
        return WaitState::Success;
    }
}

void GlFence::Reset() {
    m_state = 0;
}

void GlFence::Signal() {
    std::unique_lock<std::mutex> locker(m_lock);
    m_state = 1;
    m_condition.notify_all();
}

USING_GPU_NAMESPACE_END