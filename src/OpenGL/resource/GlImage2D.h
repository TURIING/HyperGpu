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
class GlSampler;

class GlImage2D final: public Image2D, public GlObject {
public:
    GlImage2D(OpenGlDevice* pDevice, const Image2DCreateInfo &info);
    ~GlImage2D() override;
    NODISCARD ImageUsage GetUsage() const override { return m_usage; };
    NODISCARD Size GetSize() const override { return m_size; };
    NODISCARD GlSampler* GetSampler() const { return m_pSampler; }
    NODISCARD PixelFormat GetPixelFormat() const override { return m_pixelFormat; }

private:
    void init(const Image2DCreateInfo &info);

private:
    OpenGlDevice* m_pDevice = nullptr;
    GlSampler* m_pSampler = nullptr;
    ImageUsage m_usage;
    Size m_size;
    PixelFormat m_pixelFormat;
};

USING_GPU_NAMESPACE_END

#endif //GLIMAGE2D_H
