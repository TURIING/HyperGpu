//
// Created by turiing on 2025/10/31.
//

#ifndef GLCOMPUTEPIPELINE_H
#define GLCOMPUTEPIPELINE_H

#include "../../common/common.h"
#include "GlPipeline.h"

USING_GPU_NAMESPACE_BEGIN

class GlComputePipeline final: public GlPipeline {
public:
    GlComputePipeline(OpenGlDevice* gpuDevice, const ComputeEnvInfo& computeEnvInfo);
};

USING_GPU_NAMESPACE_END

#endif //GLCOMPUTEPIPELINE_H
