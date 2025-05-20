/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/9 21:57
* @version: 1.0
* @description: 
********************************************************************************/
#include "GlContext.h"
#include "../core/OpenGlDevice.h"

#if PLATFORM_MACOS || PLATFORM_IOS
#include "platform/AGlContext.h"
#elif PLATFORM_WINDOWS
#include "platform/WGlContext.h"
#endif

USING_GPU_NAMESPACE_BEGIN

static bool g_needInit = true;
static std::mutex g_contextInitLock;

GlContext::GlContext(OpenGlDevice *pGlDevice, GlContext *shareContext, void *handle): m_pGlDevice(pGlDevice) {
    m_pGlDevice->AddRef();

    // 为了初始化glew，先创建一次临时上下文
    if(g_needInit) {
        g_contextInitLock.lock();
        this->init();
        g_needInit = false;
        g_contextInitLock.unlock();
    }

    m_pShareContext = shareContext;

    auto share = m_pShareContext ? m_pShareContext->m_pBaseContext : nullptr;
#if PLATFORM_MACOS || PLATFORM_IOS
    m_pBaseContext = AGlContext::CreateContext(dynamic_cast<AGlContext*>(share));
#elif PLATFORM_WINDOWS
	m_pBaseContext = WGlContext::CreateContext(handle, dynamic_cast<WGlContext*>(share));
#endif
}

GlContext::~GlContext() {
    m_pBaseContext->SubRef();
    m_pGlDevice->SubRef();
}

void GlContext::MakeCurrent() const {
#if PLATFORM_MACOS || PLATFORM_IOS
    dynamic_cast<AGlContext*>(m_pBaseContext)->MakeCurrent();
#elif PLATFORM_WINDOWS
	dynamic_cast<WGlContext*>(m_pBaseContext)->MakeCurrent();
#endif
}

void GlContext::ClearCurrent() const {
#if PLATFORM_MACOS || PLATFORM_IOS
    dynamic_cast<AGlContext*>(m_pBaseContext)->ClearCurrent();
#elif PLATFORM_WINDOWS
	dynamic_cast<WGlContext*>(m_pBaseContext)->ClearCurrent();
#endif
}

void GlContext::PushAndMakeCurrent() {
    m_lockContext.lock();
#if PLATFORM_MACOS || PLATFORM_IOS
    m_pPreContext = AGlContext::GetLastContext();
#elif PLATFORM_WINDOWS
	m_pPreContext = WGlContext::GetLastContext();
#endif
    this->MakeCurrent();
}

void GlContext::PopAndMakeCurrent() {
    m_pPreContext->MakeCurrent();
    m_pPreContext = nullptr;
    m_lockContext.unlock();
}

void GlContext::BindLayer(void* layer) {
#if PLATFORM_MACOS || PLATFORM_IOS
    dynamic_cast<AGlContext*>(m_pBaseContext)->BindLayer(layer);
#endif
}

void GlContext::GlSyncFinish() {
    GLenum result = GL_WAIT_FAILED;
    constexpr GLuint64 timeout = 0xffffffffffffffffull;
    GLsync sync = nullptr;
    CALL_GL(sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0));
    LOG_ASSERT(sync != nullptr);
    CALL_GL(result = glClientWaitSync(sync, GL_SYNC_FLUSH_COMMANDS_BIT, timeout));
    CALL_GL(glDeleteSync(sync));
}

bool GlContext::IsInContext() {
#if PLATFORM_MACOS || PLATFORM_IOS
    return AGlContext::GetLastContext() != nullptr;
#elif PLATFORM_WINDOWS
	return WGlContext::GetLastContext() != nullptr;
#endif
}

void GlContext::init() {
#if PLATFORM_MACOS || PLATFORM_IOS
    AGlContext::Init();
#elif PLATFORM_WINDOWS
	WGlContext::Init();
#endif

}

void GlContext::SwapBuffer() const {
#if PLATFORM_MACOS || PLATFORM_IOS
    dynamic_cast<AGlContext*>(m_pBaseContext)->SwapBuffer();
#elif PLATFORM_WINDOWS
	dynamic_cast<WGlContext*>(m_pBaseContext)->SwapBuffer();
#endif
}

USING_GPU_NAMESPACE_END