/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/17 12:04
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef GLIMAGE2D_H
#define GLIMAGE2D_H

#include "../../common/common.h"
#include "../GlObject.h"

USING_GPU_NAMESPACE_BEGIN

class OpenGlDevice;

class GlImage2D final: public Image2D, public GlObject {
public:
    GlImage2D(OpenGlDevice* pDevice, const Image2DCreateInfo &info);
    ~GlImage2D() override;
    void FillPixels(GpuCmd* pCmd, const uint8_t* data, uint64_t dataSize) override;

private:
    void init(const Image2DCreateInfo &info);

private:
    OpenGlDevice* m_pDevice = nullptr;
};

USING_GPU_NAMESPACE_END

#endif //GLIMAGE2D_H
