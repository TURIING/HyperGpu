/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/11 14:22
* @version: 1.0
* @description: 
********************************************************************************/
#include "OpenGlCmdManager.h"

GpuCmd* OpenGlCmdManager::CreateCommandBuffer() {
}

void OpenGlCmdManager::WithSingleCmdBuffer(const std::function<void(GpuCmd*)>& func) {
}

OpenGlCmdManager::~OpenGlCmdManager() {
}
