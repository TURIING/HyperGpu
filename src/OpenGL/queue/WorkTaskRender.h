/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/10 15:06
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef HYPERRENDER_WORKTASKRENDER_H
#define HYPERRENDER_WORKTASKRENDER_H

#include "../../common/common.h"
#include "WorkTask.h"

USING_GPU_NAMESPACE_BEGIN

class GlCmd;
class GlSemaphore;
class GlFence;

class WorkTaskRender final : public WorkTask {
public:
    WorkTaskRender(GlQueue* pQueue, const Queue::SubmitInfo &info);
    ~WorkTaskRender() override;
    void Execute(GlContext *pContext) override;
    WaitState Wait(uint32_t timeout) override;
    void Signal() override;

private:
    std::vector<GlCmd*> m_vecCmds;
    std::vector<GlSemaphore*> m_vecWaitSemaphore;
    std::vector<GlSemaphore*> m_vecSignalSemaphore;
    GlFence* m_pFence = nullptr;
};

USING_GPU_NAMESPACE_END
#endif //HYPERRENDER_WORKTASKRENDER_H