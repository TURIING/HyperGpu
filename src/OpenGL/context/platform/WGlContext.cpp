//
// Created by TURIING on 2025/5/18.
//

#if PLATFORM_WINDOWS
#include "WGlContext.h"

#include "include/GL/glew.h"
#include <include/GL/wglew.h>

#include <mutex>
#define WGL_DONT_CARE -1
#define WGL_TRUE 1
#define WGL_FALSE 0

namespace HyperGpu {
using WGLbool = int;


struct WGLFBConfig {
	int redBits          = 0;
	int greenBits        = 0;
	int blueBits         = 0;
	int alphaBits        = 0;
	int depthBits        = 0;
	int stencilBits      = 0;
	int accumRedBits     = 0;
	int accumGreenBits   = 0;
	int accumBlueBits    = 0;
	int accumAlphaBits   = 0;
	int auxBuffers       = 0;
	WGLbool stereo       = WGL_FALSE;
	int samples          = 0;
	WGLbool sRGB         = WGL_FALSE;
	WGLbool doublebuffer = WGL_FALSE;
	WGLbool transparent  = WGL_FALSE;
	uintptr_t handle     = 0;
};

static int findPixelFormatAttribValueWGL(const int* attribs, int attribCount,
										 const int* values, int attrib) {
	int i;

	for (i = 0; i < attribCount; i++) {
		if (attribs[i] == attrib) return values[i];
	}

	return 0;
}

#define ADD_ATTRIB(a)                                               \
{                                                                   \
assert((size_t)attribCount < sizeof(attribs) / sizeof(attribs[0])); \
attribs[attribCount++] = a;                                         \
}
#define FIND_ATTRIB_VALUE(a) \
findPixelFormatAttribValueWGL(attribs, attribCount, values, a)


const WGLFBConfig* ChooseFBConfig(const WGLFBConfig* desired,
                                  const WGLFBConfig* alternatives,
                                  unsigned int count) {
    unsigned int i;
    unsigned int missing, leastMissing     = UINT_MAX;
    unsigned int colorDiff, leastColorDiff = UINT_MAX;
    unsigned int extraDiff, leastExtraDiff = UINT_MAX;
    const WGLFBConfig* current;
    const WGLFBConfig* closest = NULL;

    for (i = 0; i < count; i++) {
        current = alternatives + i;

        if (desired->stereo > 0 && current->stereo == 0) {
            // Stereo is a hard constraint
            continue;
        }

        // Count number of missing buffers
        {
            missing = 0;

            if (desired->alphaBits > 0 && current->alphaBits == 0) missing++;

            if (desired->depthBits > 0 && current->depthBits == 0) missing++;

            if (desired->stencilBits > 0 && current->stencilBits == 0)
                missing++;

            if (desired->auxBuffers > 0
                && current->auxBuffers < desired->auxBuffers) {
                missing += desired->auxBuffers - current->auxBuffers;
            }

            if (desired->samples > 0 && current->samples == 0) {
                // Technically, several multisampling buffers could be
                // involved, but that's a lower level implementation detail and
                // not important to us here, so we count them as one
                missing++;
            }

            if (desired->transparent != current->transparent) missing++;
        }

        // These polynomials make many small channel size differences matter
        // less than one large channel size difference

        // Calculate color channel size difference value
        {
            colorDiff = 0;

            if (desired->redBits != WGL_DONT_CARE) {
                colorDiff += (desired->redBits - current->redBits)
                             * (desired->redBits - current->redBits);
            }

            if (desired->greenBits != WGL_DONT_CARE) {
                colorDiff += (desired->greenBits - current->greenBits)
                             * (desired->greenBits - current->greenBits);
            }

            if (desired->blueBits != WGL_DONT_CARE) {
                colorDiff += (desired->blueBits - current->blueBits)
                             * (desired->blueBits - current->blueBits);
            }
        }

        // Calculate non-color channel size difference value
        {
            extraDiff = 0;

            if (desired->alphaBits != WGL_DONT_CARE) {
                extraDiff += (desired->alphaBits - current->alphaBits)
                             * (desired->alphaBits - current->alphaBits);
            }

            if (desired->depthBits != WGL_DONT_CARE) {
                extraDiff += (desired->depthBits - current->depthBits)
                             * (desired->depthBits - current->depthBits);
            }

            if (desired->stencilBits != WGL_DONT_CARE) {
                extraDiff += (desired->stencilBits - current->stencilBits)
                             * (desired->stencilBits - current->stencilBits);
            }

            if (desired->accumRedBits != WGL_DONT_CARE) {
                extraDiff += (desired->accumRedBits - current->accumRedBits)
                             * (desired->accumRedBits - current->accumRedBits);
            }

            if (desired->accumGreenBits != WGL_DONT_CARE) {
                extraDiff +=
                    (desired->accumGreenBits - current->accumGreenBits)
                    * (desired->accumGreenBits - current->accumGreenBits);
            }

            if (desired->accumBlueBits != WGL_DONT_CARE) {
                extraDiff +=
                    (desired->accumBlueBits - current->accumBlueBits)
                    * (desired->accumBlueBits - current->accumBlueBits);
            }

            if (desired->accumAlphaBits != WGL_DONT_CARE) {
                extraDiff +=
                    (desired->accumAlphaBits - current->accumAlphaBits)
                    * (desired->accumAlphaBits - current->accumAlphaBits);
            }

            if (desired->samples != WGL_DONT_CARE) {
                extraDiff += (desired->samples - current->samples)
                             * (desired->samples - current->samples);
            }

            if (desired->sRGB && !current->sRGB) extraDiff++;
        }

        // Figure out if the current one is better than the best one found so
        // far Least number of missing buffers is the most important heuristic,
        // then color buffer size match and lastly size match for other buffers

        if (missing < leastMissing)
            closest = current;
        else if (missing == leastMissing) {
            if ((colorDiff < leastColorDiff)
                || (colorDiff == leastColorDiff
                    && extraDiff < leastExtraDiff)) {
                closest = current;
            }
        }

        if (current == closest) {
            leastMissing   = missing;
            leastColorDiff = colorDiff;
            leastExtraDiff = extraDiff;
        }
    }

    return closest;
}

static int choosePixelFormatWGL(HDC hdc, const WGLFBConfig* fbconfig) {
    std::vector<WGLFBConfig> usableConfigs;
    const WGLFBConfig* closest = nullptr;

    int i, pixelFormat, nativeCount, usableCount = 0, attribCount = 0;
    int attribs[40];
    int values[sizeof(attribs) / sizeof(attribs[0])];

    nativeCount = DescribePixelFormat(hdc, 1, sizeof(PIXELFORMATDESCRIPTOR), NULL);

    if (WGLEW_ARB_pixel_format) {
        ADD_ATTRIB(WGL_SUPPORT_OPENGL_ARB);
        ADD_ATTRIB(WGL_DRAW_TO_WINDOW_ARB);
        ADD_ATTRIB(WGL_PIXEL_TYPE_ARB);
        ADD_ATTRIB(WGL_ACCELERATION_ARB);
        ADD_ATTRIB(WGL_RED_BITS_ARB);
        ADD_ATTRIB(WGL_RED_SHIFT_ARB);
        ADD_ATTRIB(WGL_GREEN_BITS_ARB);
        ADD_ATTRIB(WGL_GREEN_SHIFT_ARB);
        ADD_ATTRIB(WGL_BLUE_BITS_ARB);
        ADD_ATTRIB(WGL_BLUE_SHIFT_ARB);
        ADD_ATTRIB(WGL_ALPHA_BITS_ARB);
        ADD_ATTRIB(WGL_ALPHA_SHIFT_ARB);
        ADD_ATTRIB(WGL_DEPTH_BITS_ARB);
        ADD_ATTRIB(WGL_STENCIL_BITS_ARB);
        ADD_ATTRIB(WGL_ACCUM_BITS_ARB);
        ADD_ATTRIB(WGL_ACCUM_RED_BITS_ARB);
        ADD_ATTRIB(WGL_ACCUM_GREEN_BITS_ARB);
        ADD_ATTRIB(WGL_ACCUM_BLUE_BITS_ARB);
        ADD_ATTRIB(WGL_ACCUM_ALPHA_BITS_ARB);
        ADD_ATTRIB(WGL_AUX_BUFFERS_ARB);
        ADD_ATTRIB(WGL_STEREO_ARB);
        ADD_ATTRIB(WGL_DOUBLE_BUFFER_ARB);

        if (WGLEW_ARB_multisample) ADD_ATTRIB(WGL_SAMPLES_ARB);

        if (WGLEW_ARB_framebuffer_sRGB || WGLEW_EXT_framebuffer_sRGB)
            ADD_ATTRIB(WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB);
    }

    usableConfigs.resize(nativeCount);

    for (i = 0; i < nativeCount; i++) {
        WGLFBConfig* u = usableConfigs.data() + usableCount;
        pixelFormat    = i + 1;

        if (WGLEW_ARB_pixel_format) {
            // Get pixel format attributes through "modern" extension

            if (!wglGetPixelFormatAttribivARB(hdc, pixelFormat, 0, attribCount,
                                              attribs, values)) {
                return 0;
            }

            if (!FIND_ATTRIB_VALUE(WGL_SUPPORT_OPENGL_ARB)
                || !FIND_ATTRIB_VALUE(WGL_DRAW_TO_WINDOW_ARB)) {
                continue;
            }

            if (FIND_ATTRIB_VALUE(WGL_PIXEL_TYPE_ARB) != WGL_TYPE_RGBA_ARB)
                continue;

            if (FIND_ATTRIB_VALUE(WGL_ACCELERATION_ARB)
                == WGL_NO_ACCELERATION_ARB)
                continue;

            if (FIND_ATTRIB_VALUE(WGL_DOUBLE_BUFFER_ARB)
                != fbconfig->doublebuffer)
                continue;

            u->doublebuffer = FIND_ATTRIB_VALUE(WGL_DOUBLE_BUFFER_ARB);
            u->redBits      = FIND_ATTRIB_VALUE(WGL_RED_BITS_ARB);
            u->greenBits    = FIND_ATTRIB_VALUE(WGL_GREEN_BITS_ARB);
            u->blueBits     = FIND_ATTRIB_VALUE(WGL_BLUE_BITS_ARB);
            u->alphaBits    = FIND_ATTRIB_VALUE(WGL_ALPHA_BITS_ARB);

            u->depthBits   = FIND_ATTRIB_VALUE(WGL_DEPTH_BITS_ARB);
            u->stencilBits = FIND_ATTRIB_VALUE(WGL_STENCIL_BITS_ARB);

            u->accumRedBits   = FIND_ATTRIB_VALUE(WGL_ACCUM_RED_BITS_ARB);
            u->accumGreenBits = FIND_ATTRIB_VALUE(WGL_ACCUM_GREEN_BITS_ARB);
            u->accumBlueBits  = FIND_ATTRIB_VALUE(WGL_ACCUM_BLUE_BITS_ARB);
            u->accumAlphaBits = FIND_ATTRIB_VALUE(WGL_ACCUM_ALPHA_BITS_ARB);

            u->auxBuffers = FIND_ATTRIB_VALUE(WGL_AUX_BUFFERS_ARB);

            if (FIND_ATTRIB_VALUE(WGL_STEREO_ARB)) u->stereo = true;

            if (WGLEW_ARB_multisample)
                u->samples = FIND_ATTRIB_VALUE(WGL_SAMPLES_ARB);

            if (WGLEW_ARB_framebuffer_sRGB || WGLEW_EXT_framebuffer_sRGB) {
                if (FIND_ATTRIB_VALUE(WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB))
                    u->sRGB = true;
            }
        } else {
            // Get pixel format attributes through legacy PFDs

            PIXELFORMATDESCRIPTOR pfd;

            if (!DescribePixelFormat(hdc, pixelFormat,
                                     sizeof(PIXELFORMATDESCRIPTOR), &pfd)) {
                return 0;
            }

            if (!(pfd.dwFlags & PFD_DRAW_TO_WINDOW)
                || !(pfd.dwFlags & PFD_SUPPORT_OPENGL)) {
                continue;
            }

            if (!(pfd.dwFlags & PFD_GENERIC_ACCELERATED)
                && (pfd.dwFlags & PFD_GENERIC_FORMAT)) {
                continue;
            }

            if (pfd.iPixelType != PFD_TYPE_RGBA) continue;

            if (!!(pfd.dwFlags & PFD_DOUBLEBUFFER) != fbconfig->doublebuffer)
                continue;

            u->redBits   = pfd.cRedBits;
            u->greenBits = pfd.cGreenBits;
            u->blueBits  = pfd.cBlueBits;
            u->alphaBits = pfd.cAlphaBits;

            u->depthBits   = pfd.cDepthBits;
            u->stencilBits = pfd.cStencilBits;

            u->accumRedBits   = pfd.cAccumRedBits;
            u->accumGreenBits = pfd.cAccumGreenBits;
            u->accumBlueBits  = pfd.cAccumBlueBits;
            u->accumAlphaBits = pfd.cAccumAlphaBits;

            u->auxBuffers = pfd.cAuxBuffers;

            if (pfd.dwFlags & PFD_STEREO) u->stereo = true;
        }

        u->handle = pixelFormat;
        usableCount++;
    }

    if (!usableCount) {
        return 0;
    }

    closest = ChooseFBConfig(fbconfig, usableConfigs.data(), usableCount);
    if (!closest) {
        return 0;
    }

    pixelFormat = (int)closest->handle;

    return pixelFormat;
}



std::mutex gContextMutex;
HDC gDefaultHD = nullptr;
thread_local WGlContext* gLastContext = nullptr;

void WGlContext::Init() {
	gContextMutex.lock();
	auto hdc	= ::GetDC(nullptr);
	gDefaultHD	= hdc;
	HGLRC hglrc = createContext(hdc, PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW, NULL);
	wglMakeCurrent(hdc, hglrc);
	glewExperimental = GL_TRUE;
	GLenum init		 = glewInit();
	wglDeleteContext(hglrc);
	wglMakeCurrent(NULL, NULL);
	gContextMutex.unlock();
}

WGlContext* WGlContext::CreateContext(void* handle, WGlContext* shareContext) {
	auto context = new WGlContext();
	if(handle) {
		auto hwnd	   = static_cast<HWND>(handle);
		context->m_hdc = ::GetDC(hwnd);
	} else {
		context->m_hdc = ::GetDC(nullptr);
	}

	gContextMutex.lock();
	HGLRC share		 = shareContext ? shareContext->m_hglrc : nullptr;
	context->m_hglrc = createContext(context->m_hdc, PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW, share);
	gContextMutex.unlock();

	context->m_shareContext = shareContext;
	return context;
}

HGLRC WGlContext::createContext(HDC hdc, DWORD dwFlags, HGLRC shareDc, bool doubleBuffer, bool mutilSample) {
	WGLFBConfig fbConfig;
	fbConfig.redBits   = 8;
	fbConfig.greenBits = 8;
	fbConfig.blueBits  = 8;
	fbConfig.alphaBits = 8;

	fbConfig.depthBits		= 24;
	fbConfig.stencilBits	= 8;
	fbConfig.accumRedBits	= 0;
	fbConfig.accumGreenBits = 0;
	fbConfig.accumBlueBits	= 0;
	fbConfig.accumAlphaBits = 0;
	fbConfig.auxBuffers		= 0;
	fbConfig.stereo			= 0;
	fbConfig.samples		= mutilSample ? 4 : WGL_DONT_CARE;
	fbConfig.sRGB			= WGL_FALSE;
	fbConfig.transparent	= WGL_FALSE;
	fbConfig.handle			= 0;
	fbConfig.doublebuffer	= doubleBuffer ? WGL_TRUE : WGL_FALSE;

	PIXELFORMATDESCRIPTOR pfd;
	int					  pixelFormat = choosePixelFormatWGL(hdc, &fbConfig);
	assert(pixelFormat > 0);

	if(!DescribePixelFormat(hdc, pixelFormat, sizeof(pfd), &pfd)) {
		assert(false);
	}

	if(!SetPixelFormat(hdc, pixelFormat, &pfd)) {
		assert(false);
	}

	HGLRC hglrc = NULL;
	if(WGLEW_ARB_create_context) {
		int attribtes[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB,
			4,
			WGL_CONTEXT_MINOR_VERSION_ARB,
			3,
			// WGL_CONTEXT_FLAGS_ARB, 0,
			WGL_CONTEXT_PROFILE_MASK_ARB,
			WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
			0,
			0,
		};
		// Create the OpenGL resource context (RC) and make it current to the
		// thread
		hglrc = wglCreateContextAttribsARB(hdc, shareDc, attribtes);
	} else {
		hglrc = wglCreateContext(hdc);
		if(shareDc) {
			wglShareLists(shareDc, hglrc);
		}
	}

	if(hglrc == NULL) {
		const DWORD error = GetLastError();
		assert(false);
	}

	return hglrc;
}

WGlContext::~WGlContext() {
	wglDeleteContext(m_hglrc);
}

WGlContext* WGlContext::GetLastContext() {
	return gLastContext;
}


void WGlContext::MakeCurrent() {
	if(gLastContext == this) return;

	gContextMutex.lock();
	BOOL suc = FALSE;
	HDC preHdc = nullptr;
	HGLRC preHglrc = nullptr;
	preHdc = wglGetCurrentDC();
	preHglrc = wglGetCurrentContext();

	if(preHdc != nullptr && preHglrc != nullptr) {
		if(preHdc == m_hdc && preHglrc ==  m_hglrc) {
			gContextMutex.unlock();
			return;
		}
		suc |= wglMakeCurrent(nullptr, nullptr);
		assert(suc);
	}

	suc |= wglMakeCurrent(m_hdc, m_hglrc);
	gContextMutex.unlock();
	gLastContext = this;
	assert(suc);
}

void WGlContext::ClearCurrent() {
	gContextMutex.lock();
	wglMakeCurrent(nullptr, nullptr);
	gContextMutex.unlock();
	gLastContext = nullptr;
}

void WGlContext::SwapBuffer() {
	BOOL suc = FALSE;
	suc = ::SwapBuffers(m_hdc);
	assert(suc);
}
}

#endif
