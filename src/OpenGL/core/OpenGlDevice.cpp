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
#include "OpenGlResourceManager.h"
#include "OpenGlSyncManager.h"
#include "../surface/AGlSurface.h"
#include "OpenGlCmdManager.h"
#include "../cmd/GlCmd.h"
#include "../queue/WorkTaskRender.h"

USING_GPU_NAMESPACE_BEGIN
OpenGlDevice::OpenGlDevice(const DeviceCreateInfo& info) {
    m_pMainContext = new GlContext(this, nullptr);
    for (auto i = 0; i < info.queueInfoCount; i++) {
        m_vecQueue.push_back(new GlQueue(this));
    }
    m_pOneTimeQueue = new GlQueue(this);
    m_pThreadPool = new GlThreadPool(this);
    m_pResourceManager = new OpenGlResourceManager(this);
    m_pSyncManager = new OpenGlSyncManager(this);
    m_pCmdManager = new OpenGlCmdManager(this);
}

OpenGlDevice::~OpenGlDevice() {
    for (const auto queue : m_vecQueue) {
        queue->SubRef();
    }
    m_pOneTimeQueue->SubRef();
    m_pThreadPool->SubRef();
    m_pResourceManager->SubRef();
    m_pSyncManager->SubRef();
    m_pCmdManager->SubRef();
    m_pMainContext->SubRef();
}

PipelineManager* OpenGlDevice::GetPipelineManager() {
	return nullptr;
}

GpuCmdManager* OpenGlDevice::GetCmdManager() {
	return m_pCmdManager;
}

GpuResourceManager* OpenGlDevice::GetResourceManager() {
	return m_pResourceManager;
}

GpuSyncManager* OpenGlDevice::GetSyncManager() {
	return m_pSyncManager;
}

GpuSurface *OpenGlDevice::CreateSurface(const PlatformWindowInfo &platformWindowInfo) {
#if PLATFORM_MACOS || PLATFORM_IOS
    return new AGlSurface(this, platformWindowInfo);
#endif
}

Queue *OpenGlDevice::CreateQueue(QueueType queueType) {
    return new GlQueue(this);
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

void OpenGlDevice::WithSingleCmdBuffer(std::function<void(GpuCmd* pCmd)> func) {
    GlCmd cmd(this);
    cmd.Begin();
    func(&cmd);
    cmd.End();
    GpuCmd* pCmd = &cmd;

    Queue::SubmitInfo submitInfo;
    submitInfo.pCmd = &pCmd;

    WorkTaskRender task(m_pOneTimeQueue, submitInfo);
    m_pOneTimeQueue->AddTask(&task);
    task.WaitFinish();
}

USING_GPU_NAMESPACE_END
