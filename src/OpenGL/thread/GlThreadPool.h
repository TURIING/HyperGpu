/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/10 12:27
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef HYPERRENDER_GLTHREADPOOL_H
#define HYPERRENDER_GLTHREADPOOL_H

#include "../../common/common.h"
#include "ThreadQueue.h"
#include "../queue/WorkTask.h"

USING_GPU_NAMESPACE_BEGIN
class OpenGlDevice;
class GlContext;

class GlThreadPool final : public GpuObject {
public:
    explicit GlThreadPool(OpenGlDevice* pGlDevice);
    void AddTask(WorkTask* pTask);

private:
    void threadWorkFunc(int index);

private:
    OpenGlDevice* m_pGlDevice = nullptr;
    std::vector<std::thread *> m_threads;
    std::vector<GlContext*> m_contexts;
    ThreadQueue<WorkTask> m_taskQueue;
    bool m_exit = false;
};

USING_GPU_NAMESPACE_END
#endif //HYPERRENDER_GLTHREADPOOL_H