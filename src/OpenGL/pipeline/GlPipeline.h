//
// Created by turiing on 2025/6/29.
//

#ifndef GLPIPELINE_H
#define GLPIPELINE_H

#include "../../common/common.h"

USING_GPU_NAMESPACE_BEGIN
class OpenGlDevice;
class GlProgram;

class GlPipeline: public Pipeline {
public:
    GlPipeline(OpenGlDevice* pDevice, const EnvInfo &envInfo);
    ~GlPipeline() override;
    void Bind() const;
    void SetUniforms(const UniformBinding* pUniformBinding, u32 count) const;
    void SetTextures(const ImageBinding* pImageBinding, u32 count) const;

private:
    OpenGlDevice* m_pDevice = nullptr;
    GlProgram* m_pProgram = nullptr;
};

USING_GPU_NAMESPACE_END

#endif //GLPIPELINE_H
