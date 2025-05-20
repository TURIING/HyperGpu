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
#include "../sync/GlSemaphore.h"

GlSurface::GlSurface(OpenGlDevice* pDevice, const PlatformWindowInfo &info): m_pDevice(pDevice) {
    m_pDevice->AddRef();
    m_pContext = m_pDevice->CreateContext(info.handle);
    m_pImageAvailSemaphore = new GlSemaphore();
}

GlSurface::~GlSurface() {
    m_pImageAvailSemaphore->SubRef();
	m_pContext->SubRef();
    m_pDevice->SubRef();
}

void GlSurface::Bind() const {
    m_pContext->PushAndMakeCurrent();
}

void GlSurface::Unbind() const {
    m_pContext->PopAndMakeCurrent();
}

Semaphore* GlSurface::AcquireNextImage(uint32_t& imageIndex) {
    imageIndex = 0;
    m_pImageAvailSemaphore->Signal();
    return m_pImageAvailSemaphore;
}
