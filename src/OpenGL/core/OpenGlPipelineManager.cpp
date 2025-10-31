//
// Created by turiing on 2025/6/29.
//

#include "OpenGlPipelineManager.h"

#include "OpenGlDevice.h"
#include "../pipeline/GlGraphicPipeline.h"
#include "../pipeline/GlComputePipeline.h"

OpenGlPipelineManager::OpenGlPipelineManager(OpenGlDevice *pDevice): m_pDevice(pDevice) {
    m_pDevice->AddRef();
}

OpenGlPipelineManager::~OpenGlPipelineManager() {
    m_pDevice->SubRef();
}

Pipeline* OpenGlPipelineManager::CreateGraphicPipeline(const RenderEnvInfo &renderEnvInfo) {
    return new GlGraphicPipeline(m_pDevice, renderEnvInfo);
}

Pipeline* OpenGlPipelineManager::CreateComputePipeline(const ComputeEnvInfo& computeEnvInfo) {
    return new GlComputePipeline(m_pDevice, computeEnvInfo);
}

