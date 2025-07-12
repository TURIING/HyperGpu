/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/16 22:08
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef GLSURFACE_H
#define GLSURFACE_H

#include "../../common/common.h"

USING_GPU_NAMESPACE_BEGIN
class OpenGlDevice;
class GlContext;
class GlSemaphore;

class GlSurface : public GpuSurface {
public:
    explicit GlSurface(OpenGlDevice* pDevice, const PlatformWindowInfo &info);
    ~GlSurface() override;
    Semaphore* AcquireNextImage(uint32_t& imageIndex) override;
    void Bind() const;
    void Unbind() const;
    NODISCARD virtual GLuint GetSurfaceFbo() const { return 0; }
    NODISCARD Size GetSize() const { return m_size; }
    virtual void SwapBuffers() = 0;

protected:
    OpenGlDevice* m_pDevice = nullptr;
    GlContext* m_pContext = nullptr;
    GlSemaphore* m_pImageAvailSemaphore = nullptr;
    Size m_size;
};

USING_GPU_NAMESPACE_END

#endif //GLSURFACE_H
