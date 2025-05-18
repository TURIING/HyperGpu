/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/17 16:47
* @version: 1.0
* @description: 
********************************************************************************/
#include "WorkTaskPresent.h"
#include "../surface/GlSurface.h"
#include "../sync/GlSemaphore.h"
USING_GPU_NAMESPACE_BEGIN

WorkTaskPresent::WorkTaskPresent(GlQueue* pQueue, const Queue::PresentInfo& info): WorkTask(pQueue) {
    m_pSurface = dynamic_cast<GlSurface*>(info.pSurface);
    m_pSurface->AddRef();
    m_vecWaitSemaphore.reserve(info.waitSemaphoreCount);
    for (auto  i = 0; i < info.waitSemaphoreCount; i++) {
        const auto semaphore = info.pWaitSemaphores[i];
        semaphore->AddRef();
        m_vecWaitSemaphore.push_back(dynamic_cast<GlSemaphore*>(semaphore));
    }
}

WorkTaskPresent::~WorkTaskPresent() {
    for (const auto & semaphore : m_vecWaitSemaphore) {
        semaphore->SubRef();
    }
    m_pSurface->SubRef();
}

void WorkTaskPresent::Execute(GlContext* pContext) {
    m_pSurface->Bind();
    CALL_GL(glFlush());
    m_pSurface->SwapBuffers();
    m_pSurface->Unbind();
}

WaitState WorkTaskPresent::Wait(uint32_t timeout) {
    WaitState state = WaitState::Success;

    std::vector<GlSemaphore*> curWaitSemaphores;
    curWaitSemaphores.swap(m_vecWaitSemaphore);

    for (auto it = curWaitSemaphores.begin(); it != curWaitSemaphores.end(); it++) {
        auto &semaphore = *it;
        state = semaphore->Wait(timeout);
        if (state != WaitState::Success) {
            while (it != curWaitSemaphores.end()) {
                m_vecWaitSemaphore.push_back(*it);
                ++it;
            }
            break;
        }
        else {
            semaphore->SubRef();
        }
    }
    return state;

}

void WorkTaskPresent::Signal() {
    m_pQueue->TaskFinish();
    this->SignalFinish();
}

USING_GPU_NAMESPACE_END