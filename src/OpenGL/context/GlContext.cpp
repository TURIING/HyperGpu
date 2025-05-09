/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/9 21:57
* @version: 1.0
* @description: 
********************************************************************************/
#include "GlContext.h"
#include "../core/OpenGlDevice.h"

static bool g_needInit = true;
static std::mutex g_contextInitLock;

GlContext::GlContext(OpenGlDevice *pGlDevice, GlContext *shareContext): m_pGlDevice(pGlDevice) {
    m_pGlDevice->AddRef();
    if(g_needInit) {
        g_contextInitLock.lock();
        if(g_needInit) {

        }
    }
}

GlContext::~GlContext() {
    m_pGlDevice->SubRef();
}

void GlContext::init() {
#if PLATFORM_MACOS

#endif

}
