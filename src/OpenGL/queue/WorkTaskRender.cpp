/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/10 15:06
* @version: 1.0
* @description: 
********************************************************************************/
#include "WorkTaskRender.h"
#include "../cmd/GlCmd.h"
#include "../sync/GlSemaphore.h"
#include "../sync/GlFence.h"
#include "HyperGpu/src/OpenGL/context/GlContext.h"

USING_GPU_NAMESPACE_BEGIN
WorkTaskRender::WorkTaskRender(GlQueue *pQueue, const Queue::SubmitInfo &info): WorkTask(pQueue) {
    m_vecCmds.reserve(info.cmdCount);
    for(auto i = 0; i < info.cmdCount; i++) {
        const auto pCmd = dynamic_cast<GlCmd*>(info.pCmd[i]);
        m_vecCmds.push_back(pCmd);
        pCmd->AddRef();
    }

    m_vecSignalSemaphore.reserve(info.signalSemaphoreCount);
    for(auto i = 0; i < info.signalSemaphoreCount; i++) {
        const auto semaphore = dynamic_cast<GlSemaphore*>(info.pSignalSemaphores[i]);
        m_vecSignalSemaphore.push_back(semaphore);
        semaphore->AddRef();
    }

    m_vecWaitSemaphore.reserve(info.waitSemaphoreCount);
    for (auto i = 0; i < info.waitSemaphoreCount; i++) {
        const auto semaphore = dynamic_cast<GlSemaphore*>(info.pWaitSemaphores[i]);
        m_vecWaitSemaphore.push_back(semaphore);
        semaphore->AddRef();
    }

    if (info.pFence) {
        m_pFence = dynamic_cast<GlFence*>(info.pFence);
        m_pFence->AddRef();
    }
}

WorkTaskRender::~WorkTaskRender() {
    for (auto semaphore:  m_vecSignalSemaphore) {
        semaphore->SubRef();
    }

    for (auto semaphore: m_vecWaitSemaphore) {
        semaphore->SubRef();
    }

    for (auto cmd:  m_vecCmds) {
        cmd->SubRef();
    }

    if (m_pFence) {
        m_pFence->SubRef();
    }
}

void WorkTaskRender::Execute(GlContext *pContext) {
    for (const auto pCmd:  m_vecCmds) {
        pCmd->Execute(pContext);
    }

    GlContext::GlSyncFinish();
}

WaitState WorkTaskRender::Wait(uint32_t timeout) {
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

void WorkTaskRender::Signal() {
    for (const auto semaphore: m_vecSignalSemaphore) {
        semaphore->Signal();
    }

    if (m_pFence) {
        m_pFence->Signal();
    }

    m_pQueue->TaskFinish();
    this->SignalFinish();
}

void WorkTaskRender::Reset() {
    for (const auto &pCmd:  m_vecCmds) {
        pCmd->Reset();
    }
}

USING_GPU_NAMESPACE_END
