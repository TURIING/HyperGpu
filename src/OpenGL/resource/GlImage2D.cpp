/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/17 12:04
* @version: 1.0
* @description: 
********************************************************************************/
#include "GlImage2D.h"
#include "../core/OpenGlDevice.h"
#include "../resource/GlSampler.h"

USING_GPU_NAMESPACE_BEGIN

GlImage2D::GlImage2D(OpenGlDevice* pDevice, const Image2DCreateInfo& info): m_pDevice(pDevice), m_aspect(info.aspect), m_usage(info.usage), m_size(info.size), m_pixelFormat(info.format) {
    m_pDevice->AddRef();
    LOG_ASSERT(info.pSampler);
    m_pSampler = dynamic_cast<GlSampler *>(info.pSampler);
    m_pSampler->AddRef();

    m_pDevice->RunWithContext([&](GlContext* pContext) {
        this->init(info);
    });
}

GlImage2D::~GlImage2D() {
    m_pDevice->RunWithContext([=](GlContext* pContext) {
        CALL_GL(glDeleteTextures(1, &m_handle));
    }, false);
    m_pSampler->SubRef();
    m_pDevice->SubRef();
}

void GlImage2D::init(const Image2DCreateInfo& info) {
    LOG_ASSERT(static_cast<int>(info.format) < std::size(gPixelFormatToGlFormat));
    LOG_ASSERT(static_cast<int>(info.format) < std::size(gPixelFormatToDataFormat));

    const auto texFormat = gPixelFormatToGlFormat[static_cast<int>(info.format)];
    const auto dataFormat = gPixelFormatToDataFormat[static_cast<int>(info.format)];

    CALL_GL(glGenTextures(1, &m_handle));
    CALL_GL(glBindTexture(GL_TEXTURE_2D, m_handle));
    CALL_GL(glTexImage2D(GL_TEXTURE_2D, 0, texFormat, info.size.width, info.size.height, 0, texFormat, dataFormat, nullptr));
    // CALL_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    // CALL_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    // CALL_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    // CALL_GL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    CALL_GL(glBindTexture(GL_TEXTURE_2D, 0));
}

USING_GPU_NAMESPACE_END