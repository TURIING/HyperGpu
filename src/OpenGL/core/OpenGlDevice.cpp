/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/2/22 22:33
* @version: 1.0
* @description: 
********************************************************************************/

#include "OpenGlDevice.h"
#include "../context/GlContext.h"
#include "../queue/GlQueue.h"
#include "../queue/WorkTaskCustom.h"
#include "../thread/GlThreadPool.h"

USING_GPU_NAMESPACE_BEGIN

OpenGlDevice::OpenGlDevice(const DeviceCreateInfo& info) {
    m_pMainContext = new GlContext(this, nullptr);
    for (auto i = 0; i < info.queueInfoCount; i++) {
        m_vecQueue.push_back(new GlQueue(this));
    }
    m_pOneTimeQueue = new GlQueue(this);
    m_pThreadPool = new GlThreadPool(this);
}

OpenGlDevice::~OpenGlDevice() {
    m_pMainContext->SubRef();
    for (const auto queue : m_vecQueue) {
        queue->SubRef();
    }
    m_pOneTimeQueue->SubRef();
}

PipelineManager* OpenGlDevice::GetPipelineManager() {
	return nullptr;
}

GpuCmdManager* OpenGlDevice::GetCmdManager() {
	return nullptr;
}

GpuResourceManager* OpenGlDevice::GetResourceManager() {
	return nullptr;
}

GpuSyncManager* OpenGlDevice::GetSyncManager() {
	return nullptr;
}

GpuSurface *OpenGlDevice::CreateSurface(const PlatformWindowInfo &platformWindowInfo) {
    return nullptr;
}

Queue *OpenGlDevice::CreateQueue(QueueType queueType) {
    return nullptr;
}

GlContext *OpenGlDevice::CreateContext() {
    return new GlContext(this, m_pMainContext);
}

void OpenGlDevice::RunWithContext(std::function<void(GlContext*)> func, bool waitFinish) const {
    if (GlContext::IsInContext()) {
        func(m_pMainContext);
        if (waitFinish) {
            GlContext::GlSyncFinish();
        }
        else {
            CALL_GL(glFlush());
        }
    }
    else {
        if (!waitFinish) {
            const auto task = new WorkTaskCustom(m_pOneTimeQueue,  func);
            if (!m_pOneTimeQueue->AddTask(task)) {
                task->SubRef();
            }
        }
        else {
            WorkTaskCustom task(m_pOneTimeQueue, func);
            m_pOneTimeQueue->AddTask(&task);
            task.WaitFinish();
        }
    }
}

USING_GPU_NAMESPACE_END