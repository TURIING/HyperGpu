/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/10 14:43
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef HYPERRENDER_WORKTASK_H
#define HYPERRENDER_WORKTASK_H

#include "../../common/common.h"
#include "GlQueue.h"

USING_GPU_NAMESPACE_BEGIN
class GlContext;

class WorkTask: public GpuObject {
public:
    explicit WorkTask(GlQueue* pQueue): m_pQueue(pQueue) {
        m_pQueue->AddRef();
    };

    ~WorkTask() override {
        m_pQueue->SubRef();
    }

    virtual void Execute(GlContext* pContext) = 0;
    virtual WaitState Wait(uint32_t timeout = 0) = 0;
    virtual void Signal() = 0;
    virtual void Reset() = 0;
    void WaitFinish() {
        std::unique_lock<std::mutex> locker(m_lock);
        m_condition.wait(locker, [&] { return m_finish; });
    }

    void SignalFinish() {
        std::unique_lock<std::mutex> locker(m_lock);
        m_finish = true;
        m_condition.notify_all();
    }

protected:
    GlQueue* m_pQueue = nullptr;
    std::mutex m_lock;
    std::condition_variable m_condition;
    bool m_finish = false;
};

USING_GPU_NAMESPACE_END
#endif //HYPERRENDER_WORKTASK_H
