/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/17 16:47
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef WORKTASKPRESENT_H
#define WORKTASKPRESENT_H

#include "../../common/common.h"
#include "WorkTask.h"

USING_GPU_NAMESPACE_BEGIN

class GlQueue;
class GlSurface;
class GlSemaphore;

class WorkTaskPresent final: public WorkTask {
public:
    WorkTaskPresent(GlQueue* pQueue, const Queue::PresentInfo &info);
    ~WorkTaskPresent() override;
    void Execute(GlContext* pContext) override;
    WaitState Wait(uint32_t timeout) override;
    void Signal() override;

private:
    GlSurface* m_pSurface = nullptr;
    std::vector<GlSemaphore*> m_vecWaitSemaphore;
};

USING_GPU_NAMESPACE_END

#endif //WORKTASKPRESENT_H
