//
// Created by turiing on 2025/7/19.
//

#include "GlSampler.h"
#include "../core/OpenGlDevice.h"

USING_GPU_NAMESPACE_BEGIN

GlSampler::GlSampler(OpenGlDevice *pDevice, const SamplerCreateInfo &info): m_pDevice(pDevice) {
    m_pDevice->AddRef();
    m_pDevice->RunWithContext([&](GlContext*) {
        CALL_GL(glGenSamplers(1, &m_handle));
        CALL_GL(glSamplerParameteri(m_handle, GL_TEXTURE_WRAP_S, gAddressModeToGLAddressMode[TO_U32(info.addressModeU)]));
        CALL_GL(glSamplerParameteri(m_handle, GL_TEXTURE_WRAP_T, gAddressModeToGLAddressMode[TO_U32(info.addressModeV)]));
        CALL_GL(glSamplerParameteri(m_handle, GL_TEXTURE_WRAP_R, gAddressModeToGLAddressMode[TO_U32(info.addressModeW)]));
        CALL_GL(glSamplerParameteri(m_handle, GL_TEXTURE_MIN_FILTER, gFilterToGlFilter[TO_U32(info.minFilter)]));
        CALL_GL(glSamplerParameteri(m_handle, GL_TEXTURE_MAG_FILTER, gFilterToGlFilter[TO_U32(info.magFilter)]));
    });
}

GlSampler::~GlSampler() {
    m_pDevice->RunWithContext([=](GlContext*) {
        CALL_GL(glDeleteSamplers(1, &m_handle));
    }, false);
    m_pDevice->SubRef();
}

USING_GPU_NAMESPACE_END
