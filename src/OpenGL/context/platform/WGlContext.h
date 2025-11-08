//
// Created by TURIING on 2025/5/18.
//

#ifndef WGLCONTEXT_H
#define WGLCONTEXT_H

#if PLATFORM_WINDOWS
#include "IContext.h"
#include <Windows.h>
#include <EGL/egl.h>

namespace HyperGpu {
class WGlContext final: public IContext{
public:
	~WGlContext() override;
	void MakeCurrent() override;
	void ClearCurrent() override;
	static void Init();
	void SwapBuffer() override;
	static WGlContext* CreateContext(void *handle, WGlContext* shareContext);
	static WGlContext* GetLastContext();

private:
#if WGL_CONTEXT
	static HGLRC createContext(HDC hdc, DWORD dwFlags, HGLRC shareDc, bool doubleBuffer = false, bool mutilSample = false);
#else
	static std::tuple<EGLContext, EGLDisplay, EGLSurface> createContext(HDC hdc, void* handle, EGLContext shareContext, bool doubleBuffer = false, bool mutilSample = false);
#endif

private:
	HDC m_hdc = nullptr;
	HWND m_hwnd = nullptr;
#if WGL_CONTEXT
	HGLRC m_hglrc = nullptr;
#else
	EGLContext m_eglContext = EGL_NO_CONTEXT;
	EGLDisplay m_eglDisplay = EGL_NO_DISPLAY;
	EGLSurface m_eglSurface = EGL_NO_SURFACE;
#endif
	WGlContext* m_shareContext = nullptr;
};
}
#endif
#endif //WGLCONTEXT_H
