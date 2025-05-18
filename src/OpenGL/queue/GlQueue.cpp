/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/10 14:45
* @version: 1.0
* @description: 
********************************************************************************/
#include "GlQueue.h"

#include "../core/OpenGlDevice.h"
#include "../thread/GlThreadPool.h"
#include "../queue/WorkTaskPresent.h"
#include "../queue/WorkTaskRender.h"

USING_GPU_NAMESPACE_BEGIN
GlQueue::~GlQueue() {
}

void GlQueue::Submit(const SubmitInfo& submitInfo) {
    const auto task = new WorkTaskRender(this, submitInfo);
    this->AddTask(task);
    task->SubRef();
}

void GlQueue::Present(const PresentInfo& presentInfo) {
    const auto task = new WorkTaskPresent(this, presentInfo);
    this->AddTask(task);
    task->SubRef();
}

void GlQueue::Wait() {
    if (m_count <= 0) return;

    std::unique_lock<std::mutex> locker(m_lock);
    m_condition.wait(locker, [&] { return m_count <= 0; });
}

void GlQueue::TaskFinish() {
    --m_count;
    std::unique_lock<std::mutex> locker(m_lock);
    m_condition.notify_all();
}

bool GlQueue::AddTask(WorkTask* task) {
    LOG_ASSERT(task);
    if (m_exit) {
        return false;
    }

    m_pDevice->GetThreadPool()->AddTask(task);
    ++m_count;
    return true;
}

USING_GPU_NAMESPACE_END