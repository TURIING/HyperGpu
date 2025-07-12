//
// Created by turiing on 2025/6/29.
//

#ifndef GLPIPELINE_H
#define GLPIPELINE_H

#include "../../common/common.h"

USING_GPU_NAMESPACE_BEGIN
class OpenGlDevice;
class GlProgram;

class GlPipeline final: public Pipeline {
public:
    GlPipeline(OpenGlDevice* pDevice, const RenderEnvInfo &renderEnvInfo);
    ~GlPipeline() override;
    void Bind() const;
    NODISCARD PrimitiveType GetPrimitiveType() const { return m_primitiveType; }

private:
    OpenGlDevice* m_pDevice = nullptr;
    GlProgram* m_pProgram = nullptr;
    PrimitiveType m_primitiveType;
};

USING_GPU_NAMESPACE_END

#endif //GLPIPELINE_H
