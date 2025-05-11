/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/2/19 22:44
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <set>

#include <vulkan/vulkan.h>

#if !defined(PLATFORM_MACOS) && !defined(PLATFORM_IOS)
#include <glew/include/GL/glew.h>
#elif PLATFORM_MACOS
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#elif PLATFORM_IOS
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>
#endif

#include "BaseDefine.h"
#include "Gpu.h"
#include "HyperGpu.h"
#include "LogManager.h"
#include "utility.h"

#endif //COMMON_H
