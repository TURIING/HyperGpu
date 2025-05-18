/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/10 15:20
* @version: 1.0
* @description: 
********************************************************************************/
#include "GlSemaphore.h"

WaitState GlSemaphore::Wait(u32 timeout) {
    std::unique_lock<std::mutex> locker(m_lock);
    if (m_count > 0) {
        --m_count;
        return WaitState::Success;
    }

    if (timeout > 0) {
        while (true) {
            if (m_condition.wait_for(locker, std::chrono::nanoseconds(timeout), [&]{ return m_count > 0; })) {
                --m_count;
                return WaitState::Success;
            }
            return WaitState::Timeout;
        }
    }
    else {
        m_condition.wait(locker, [=] { return m_count > 0; });
        --m_count;
        return WaitState::Success;
    }
}

void GlSemaphore::Signal() {
    std::unique_lock<std::mutex> locker(m_lock);
    ++m_count;
    m_condition.notify_one();
}

void GlSemaphore::Reset() {
    m_count = 0;
}
