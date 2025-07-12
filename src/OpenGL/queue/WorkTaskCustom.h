/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/11 12:11
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef WORKTASKCUSTOM_H
#define WORKTASKCUSTOM_H

#include "WorkTask.h"
#include "../../common/common.h"

class WorkTaskCustom final: public WorkTask {
public:
    using TaskFunc = std::function<void(GlContext* context)>;
    WorkTaskCustom(GlQueue* pQueue, const TaskFunc &func): WorkTask(pQueue), m_func(func) {};
    void Execute(GlContext* pContext) override;
    WaitState Wait(uint32_t timeout) override;
    void Reset() override {};
    void Signal() override;

private:
    TaskFunc m_func;
};



#endif //WORKTASKCUSTOM_H
