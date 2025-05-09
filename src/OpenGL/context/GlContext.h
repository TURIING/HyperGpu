/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/9 21:57
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef HYPERRENDER_GLCONTEXT_H
#define HYPERRENDER_GLCONTEXT_H

#include "../../common/common.h"

class OpenGlDevice;

class GlContext final : public GpuObject {
public:
    GlContext(OpenGlDevice* pGlDevice, GlContext* shareContext);
    ~GlContext();

private:
    void init();

private:
    OpenGlDevice* m_pGlDevice = nullptr;
};


#endif //HYPERRENDER_GLCONTEXT_H
