//
// Created by turiing on 2025/6/29.
//

#ifndef OPENGLPIPELINEMANAGER_H
#define OPENGLPIPELINEMANAGER_H

#include "../../common/common.h"

USING_GPU_NAMESPACE_BEGIN
class OpenGlDevice;

class OpenGlPipelineManager final: public PipelineManager {
public:
    explicit OpenGlPipelineManager(OpenGlDevice* pDevice);
    ~OpenGlPipelineManager() override;
    NODISCARD Pipeline* CreateRenderPipeline(const RenderEnvInfo &renderEnvInfo) override;

private:
    OpenGlDevice* m_pDevice = nullptr;
};

USING_GPU_NAMESPACE_END

#endif //OPENGLPIPELINEMANAGER_H
