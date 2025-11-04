//
// Created by turiing on 2025/10/31.
//

#ifndef GLGRAPHICPIPELINE_H
#define GLGRAPHICPIPELINE_H

#include "../../common/common.h"
#include "GlPipeline.h"

USING_GPU_NAMESPACE_BEGIN

class GlGraphicPipeline final: public GlPipeline {
public:
    GlGraphicPipeline(OpenGlDevice* pDevice, const RenderEnvInfo &renderEnvInfo);
    NODISCARD PrimitiveType GetPrimitiveType() const { return m_primitiveType; }
    NODISCARD const BlendInfo& GetBlendInfo() const { return m_blendInfo; }

private:
    PrimitiveType m_primitiveType;
    BlendInfo m_blendInfo;
};

USING_GPU_NAMESPACE_END

#endif //GLGRAPHICPIPELINE_H
