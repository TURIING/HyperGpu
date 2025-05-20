//
// Created by TURIING on 2025/5/19.
//

#ifndef WGLSURFACE_H
#define WGLSURFACE_H

#include "../context/GlContext.h"
#include "GlSurface.h"

class WGlSurface final: public GlSurface {
public:
	WGlSurface(OpenGlDevice* pDevice, const PlatformWindowInfo &info): GlSurface(pDevice, info) {}
	void SwapBuffers() override {
		m_pContext->SwapBuffer();
	}
};
#endif //WGLSURFACE_H
