/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/16 22:08
* @version: 1.0
* @description: 
********************************************************************************/
#include "GlSurface.h"

#include "../core/OpenGlDevice.h"
#include "../context/GlContext.h"

GlSurface::GlSurface(OpenGlDevice* pDevice, const PlatformWindowInfo& info): m_pDevice(pDevice) {
    m_pDevice->AddRef();
    m_pContext = m_pDevice->CreateContext();
}

GlSurface::~GlSurface() {
    m_pDevice->SubRef();
}

void GlSurface::Bind() const {
    m_pContext->PushAndMakeCurrent();
}

void GlSurface::Unbind() const {
    m_pContext->PopAndMakeCurrent();
}


Semaphore* GlSurface::AcquireNextImage(uint32_t& imageIndex) {
}
