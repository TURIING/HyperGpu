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

WorkTaskPresent::WorkTaskPresent(GlQueue* pQueue, const Queue::PresentInfo& info) {
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

}

WaitState WorkTaskPresent::Wait(uint32_t timeout) {
}

void WorkTaskPresent::Signal() {
}
