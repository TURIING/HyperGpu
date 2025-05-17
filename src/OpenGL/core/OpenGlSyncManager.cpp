/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/17 15:20
* @version: 1.0
* @description: 
********************************************************************************/
#include "OpenGlSyncManager.h"
#include "OpenGlDevice.h"
#include "../sync/GlFence.h"
#include "../sync/GlSemaphore.h"

OpenGlSyncManager::OpenGlSyncManager(OpenGlDevice* pDevice): m_pDevice(pDevice) {
    m_pDevice->AddRef();
}

OpenGlSyncManager::~OpenGlSyncManager() {
    m_pDevice->SubRef();
}

Fence* OpenGlSyncManager::CreateFence() {
    return new GlFence();
}

Semaphore* OpenGlSyncManager::CreateSemaphore() {
    return new GlSemaphore();
}
