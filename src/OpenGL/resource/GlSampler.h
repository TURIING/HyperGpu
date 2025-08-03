//
// Created by turiing on 2025/7/19.
//

#ifndef GLSAMPLER_H
#define GLSAMPLER_H

#include "../../common/common.h"
#include "../GlObject.h"

USING_GPU_NAMESPACE_BEGIN

class OpenGlDevice;

class GlSampler final: public Sampler, public GlObject {
public:
    GlSampler(OpenGlDevice* pDevice, const SamplerCreateInfo& info);
    ~GlSampler();

private:
    OpenGlDevice* m_pDevice = nullptr;
};

USING_GPU_NAMESPACE_END
#endif //GLSAMPLER_H
