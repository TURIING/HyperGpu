/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/10 14:45
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef HYPERRENDER_GLQUEUE_H
#define HYPERRENDER_GLQUEUE_H

#include "../../common/common.h"

USING_GPU_NAMESPACE_BEGIN
class WorkTask;
class OpenGlDevice;

class GlQueue final : public Queue {
public:
    explicit GlQueue(OpenGlDevice* pDevice): m_pDevice(pDevice) {}
    ~GlQueue() override;
    void Submit(const SubmitInfo& submitInfo) override;
    void Present(const PresentInfo& presentInfo) override;
    void Wait() override;
    void TaskFinish();
    bool AddTask(WorkTask* task);
    void Exit() { m_exit = true; }

private:
    OpenGlDevice* m_pDevice = nullptr;
    std::mutex m_lock;
    std::condition_variable m_condition;
    std::atomic<int32_t> m_count = 0;
    std::atomic<bool> m_exit = false;
};

USING_GPU_NAMESPACE_END
#endif //HYPERRENDER_GLQUEUE_H
