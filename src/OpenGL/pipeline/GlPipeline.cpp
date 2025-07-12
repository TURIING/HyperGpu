//
// Created by turiing on 2025/6/29.
//

#include "GlPipeline.h"

#include "../core/OpenGlDevice.h"
#include "GlProgram.h"

GlPipeline::GlPipeline(OpenGlDevice *pDevice, const RenderEnvInfo &renderEnvInfo): m_pDevice(pDevice) {
    m_pDevice->AddRef();
    m_pProgram = new GlProgram(m_pDevice, renderEnvInfo.shaderInfo);
    m_primitiveType = renderEnvInfo.rasterInfo.primitiveType;
}

GlPipeline::~GlPipeline() {
    m_pProgram->SubRef();
    m_pDevice->SubRef();
}

void GlPipeline::Bind() const {
    m_pProgram->Bind();
}
