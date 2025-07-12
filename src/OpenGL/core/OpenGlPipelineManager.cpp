//
// Created by turiing on 2025/6/29.
//

#include "OpenGlPipelineManager.h"

#include "OpenGlDevice.h"
#include "../pipeline/GlPipeline.h"

OpenGlPipelineManager::OpenGlPipelineManager(OpenGlDevice *pDevice): m_pDevice(pDevice) {
    m_pDevice->AddRef();
}

OpenGlPipelineManager::~OpenGlPipelineManager() {
    m_pDevice->SubRef();
}

Pipeline * OpenGlPipelineManager::CreateRenderPipeline(const RenderEnvInfo &renderEnvInfo) {
    return new GlPipeline(m_pDevice, renderEnvInfo);
}

