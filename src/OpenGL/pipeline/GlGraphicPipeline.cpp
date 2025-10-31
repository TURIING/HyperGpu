//
// Created by turiing on 2025/10/31.
//

#include "GlGraphicPipeline.h"

USING_GPU_NAMESPACE_BEGIN

GlGraphicPipeline::GlGraphicPipeline(OpenGlDevice *pDevice, const RenderEnvInfo &renderEnvInfo): GlPipeline(pDevice, renderEnvInfo) {
    m_primitiveType = renderEnvInfo.rasterInfo.primitiveType;
}

USING_GPU_NAMESPACE_END
