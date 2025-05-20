//
// Created by TURIING on 2025/5/18.
//

#ifndef WGLCONTEXT_H
#define WGLCONTEXT_H

#if PLATFORM_WINDOWS
#include "IContext.h"
#include <Windows.h>

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
	static HGLRC createContext(HDC hdc, DWORD dwFlags, HGLRC shareDc, bool doubleBuffer = false, bool mutilSample = false);

private:
	HDC m_hdc = nullptr;
	HGLRC m_hglrc = nullptr;
	WGlContext* m_shareContext = nullptr;
};
}
#endif
#endif //WGLCONTEXT_H
