/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/17 12:08
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef OPENGLRESOURCEMANAGER_H
#define OPENGLRESOURCEMANAGER_H

#include "../../common/common.h"

USING_GPU_NAMESPACE_BEGIN
class OpenGlDevice;

class OpenGlResourceManager final: public GpuResourceManager {
public:
    explicit OpenGlResourceManager(OpenGlDevice* pDevice);
    ~OpenGlResourceManager() override;
    [[nodiscard]] Image2D* CreateImage2D(const Image2D::Image2DCreateInfo& info) override;
    [[nodiscard]] Buffer*  CreateBuffer(const Buffer::BufferCreateInfo& createInfo) override;
    [[nodiscard]] Sampler* CreateSampler(const Sampler::SamplerCreateInfo& info) override;

private:
    OpenGlDevice* m_pDevice = nullptr;
};


USING_GPU_NAMESPACE_END
#endif //OPENGLRESOURCEMANAGER_H
