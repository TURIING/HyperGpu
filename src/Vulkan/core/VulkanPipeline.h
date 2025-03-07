/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 23:00
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULKANPIPELINE_H
#define VULKANPIPELINE_H

#include "../../common/common.h"
#include "GpuPipeline.h"

using namespace HyperGpu;

class VulPipeLineLayout;
class VulkanDevice;

class VulkanPipeline final : public Pipeline {
public:
    VulkanPipeline(const std::shared_ptr<VulkanDevice> &pDevice, const RenderEnvInfo &renderEnvInfo);
    ~VulkanPipeline() override;

private:
    std::unique_ptr<VulPipeLineLayout> m_pPipelineLayout;
};

#endif //VULKANPIPELINE_H
