/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 23:26
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULKANPIPELINEMANAGER_H
#define VULKANPIPELINEMANAGER_H

#include "../../common/common.h"
#include "GpuPipeline.h"

using namespace HyperGpu;
class VulkanDevice;

class VulkanPipelineManager final : public PipelineManager {
public:
    explicit VulkanPipelineManager(const std::shared_ptr<VulkanDevice>& device);
    std::shared_ptr<Pipeline> CreateRenderPipeline(const RenderEnvInfo& renderEnvInfo) override;

private:
    std::shared_ptr<VulkanDevice> m_pDevice;
};
#endif //VULKANPIPELINEMANAGER_H
