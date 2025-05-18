#include "AGlContext.h"

#if PLATFORM_MACOS
#import <AppKit/AppKit.h>
#elif PLATFORM_IOS
#import <UIKit/UIKit.h>
#endif

namespace HyperGpu {
thread_local AGlContext* gLastContext = nullptr;

void AGlContext::init() {

}

AGlContext* AGlContext::CreateContext(AGlContext* shareContext) {
	auto context = new AGlContext();
    auto ctx = shareContext == nullptr ? nullptr : shareContext->m_pContext;
	context->createContext(ctx);
	return context;
}

void AGlContext::createContext(void* shareContext) {
#if PLATFORM_IOS
    EAGLContext *target = nil;
    if (shareContext == nullptr)
    {
        target = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
    }
    else
    {
        EAGLContext *sharedCtx = (__bridge EAGLContext *)shareContext;
        target = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3 sharegroup:sharedCtx.sharegroup];
    }
    m_pContext = (void *)CFBridgingRetain(target);
#elif PLATFORM_MACOS
    NSOpenGLContext *target = nil;
    static NSOpenGLPixelFormatAttribute kDefaultAttributes[] = {
        NSOpenGLPFADoubleBuffer, //双缓冲
        NSOpenGLPFADepthSize, 24, //深度缓冲位深
        NSOpenGLPFAStencilSize, 8, //模板缓冲位深
        NSOpenGLPFAMultisample, //多重采样
        NSOpenGLPFASampleBuffers, (NSOpenGLPixelFormatAttribute)1, //多重采样buffer
        NSOpenGLPFASamples, (NSOpenGLPixelFormatAttribute)4, // 多重采样数
        NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core, // OpenGL3.2
        0
    };
    static NSOpenGLPixelFormat* pixelFormat = [[NSOpenGLPixelFormat alloc] initWithAttributes: kDefaultAttributes];


    NSOpenGLPixelFormat *pf = [NSOpenGLView defaultPixelFormat];
    if (shareContext == nullptr)
    {
        target = [[NSOpenGLContext alloc] initWithFormat:pixelFormat shareContext:nil];
    }
    else
    {
        auto *sharedCtx = (__bridge NSOpenGLContext *)shareContext;
        target = [[NSOpenGLContext alloc] initWithFormat:pixelFormat shareContext:sharedCtx];;
    }
    m_pContext = (void *)CFBridgingRetain(target);
#endif
}

AGlContext::~AGlContext() {
#if PLATFORM_IOS
    if ([EAGLContext currentContext] == m_pContext)
    {
        CFBridgingRelease(m_pContext);
        [EAGLContext setCurrentContext:nil];
    }
#elif PLATFORM_MACOS
    if (m_pContext != nullptr)
    {
        CFBridgingRelease(m_pContext);
        [NSOpenGLContext clearCurrentContext];
    }
#endif
}

void AGlContext::MakeCurrent() {
	gLastContext = this;

#if PLATFORM_IOS
    if(m_pContext != nullptr) {
        EAGLContext *tempCtx = (__bridge EAGLContext *)m_pContext;
        [EAGLContext setCurrentContext:tempCtx];
    }
    else {
        [EAGLContext setCurrentContext:nullptr];
    }
#elif PLATFORM_MACOS
    if(m_pContext != nullptr)
    {
        NSOpenGLContext *tempCtx = (__bridge NSOpenGLContext *)m_pContext;
        [tempCtx makeCurrentContext];
    }
#endif
}

void AGlContext::ClearCurrent() {
	gLastContext = nullptr;

#if PLATFORM_IOS
    [EAGLContext setCurrentContext:nullptr];
#elif PLATFORM_MACOS
#endif
}

AGlContext* AGlContext::GetLastContext() {
	return gLastContext;
}

void AGlContext::BindLayer(void *layer) {
#if PLATFORM_IOS
    if(layer != nullptr && m_pContext != nullptr)
    {
        [(__bridge EAGLContext *)m_pContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(__bridge CAEAGLLayer *)layer];
    }
#else
    if(layer != nullptr && m_pContext != nullptr)
    {
        NSOpenGLContext *tempCtx = (__bridge NSOpenGLContext *)m_pContext;
        NSView *tempView = (__bridge NSView *)layer;
        if ([NSThread isMainThread] == NO)
        {
            dispatch_async(dispatch_get_main_queue(), ^{
                [tempCtx setView:tempView];
            });
        }
        else
        {
            [tempCtx setView:tempView];
        }
    }
#endif
}

void AGlContext::SwapBuffer() {
#if PLATFORM_IOS
    [(__bridge EAGLContext *)m_pContext presentRenderbuffer:GL_RENDERBUFFER];
#else
    if(m_pContext != nullptr)
    {
        NSOpenGLContext *tempCtx = (__bridge NSOpenGLContext *)m_pContext;
        [tempCtx flushBuffer];
    }
#endif
}
}