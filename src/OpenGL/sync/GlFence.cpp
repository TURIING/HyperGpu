/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/10 15:21
* @version: 1.0
* @description: 
********************************************************************************/
#include "GlFence.h"

void GlFence::Wait() const {
}

void GlFence::Reset() const {
}

void GlFence::Signal() {
    std::unique_lock<std::mutex> locker(m_lock);
    m_state = 1;
    m_condition.notify_all();
}
