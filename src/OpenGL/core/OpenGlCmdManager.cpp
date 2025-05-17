/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/11 14:22
* @version: 1.0
* @description: 
********************************************************************************/
#include "OpenGlCmdManager.h"
#include "../cmd/GlCmd.h"

GpuCmd* OpenGlCmdManager::CreateCommandBuffer() {
    return new GlCmd(m_pDevice);
}

void OpenGlCmdManager::WithSingleCmdBuffer(const std::function<void(GpuCmd*)>& func) {
}

OpenGlCmdManager::~OpenGlCmdManager() {
}
