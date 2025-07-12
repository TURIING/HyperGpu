//
// Created by turiing on 2025/6/28.
//

#ifndef GLSHADER_H
#define GLSHADER_H

#include "../../common/common.h"
#include "../GlObject.h"

USING_GPU_NAMESPACE_BEGIN

class OpenGlDevice;

class GlShader final: public GpuObject, public GlObject {
public:
    enum ShaderStage { Vertex, Fragment, Geometry, Compute };
    GlShader(OpenGlDevice* pDevice, const char *data, ShaderStage stage);
    ~GlShader() override;

private:
    OpenGlDevice* m_pDevice = nullptr;
};

USING_GPU_NAMESPACE_END

#endif //GLSHADER_H
