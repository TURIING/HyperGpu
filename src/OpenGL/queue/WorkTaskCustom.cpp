/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/11 12:11
* @version: 1.0
* @description: 
********************************************************************************/
#include "WorkTaskCustom.h"

#include "../context/GlContext.h"

void WorkTaskCustom::Execute(GlContext* pContext) {
    m_func(pContext);
    GlContext::GlSyncFinish();
}

WaitState WorkTaskCustom::Wait(uint32_t timeout) {
    return WaitState::Success;
}

void WorkTaskCustom::Signal() {
    m_pQueue->TaskFinish();
    this->SignalFinish();
}
