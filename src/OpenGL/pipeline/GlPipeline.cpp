//
// Created by turiing on 2025/6/29.
//

#include "GlPipeline.h"

#include "../core/OpenGlDevice.h"
#include "GlProgram.h"

USING_GPU_NAMESPACE_BEGIN

GlPipeline::GlPipeline(OpenGlDevice *pDevice, const EnvInfo &envInfo): m_pDevice(pDevice) {
    m_pDevice->AddRef();
    m_pProgram = new GlProgram(m_pDevice, envInfo.shaderInfo);
}

GlPipeline::~GlPipeline() {
    m_pProgram->SubRef();
    m_pDevice->SubRef();
}

void GlPipeline::Bind() const {
    m_pProgram->Bind();
}

USING_GPU_NAMESPACE_END