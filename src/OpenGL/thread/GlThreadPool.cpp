/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/10 12:27
* @version: 1.0
* @description: 
********************************************************************************/
#include "GlThreadPool.h"
#include "../context/GlContext.h"
#include "../core/OpenGlDevice.h"

GlThreadPool::GlThreadPool(OpenGlDevice *pGlDevice): m_pGlDevice(pGlDevice) {
    m_threads.reserve(THREAD_NUM);
    m_contexts.reserve(THREAD_NUM);

    for (auto i = 0; i < THREAD_NUM; i++) {
        m_contexts.push_back(m_pGlDevice->CreateContext());
        m_threads.push_back( new std::thread(&GlThreadPool::threadWorkFunc, this, i));
    }
}

void GlThreadPool::AddTask(WorkTask* pTask) {
    m_taskQueue.AddItem(pTask);
}

void GlThreadPool::threadWorkFunc(int index) {
    auto ctx = m_contexts[index];
    ctx->MakeCurrent();

    while (!m_exit) {
        auto task = m_taskQueue.WaitItem();
        LOG_ASSERT(task);

        WaitState state = WaitState::Success;
        state = task->Wait(1000);
        if (state == WaitState::Success) {
            task->Execute(ctx);
            task->Signal();
            break;
        }
        task->SubRef();
    }


}
