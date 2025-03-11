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

class VulPipeline;
class VulRenderPass;
using namespace HyperGpu;

class VulPipeLineLayout;
class VulkanDevice;

class VulkanPipeline final : public Pipeline {
public:
    VulkanPipeline(const std::shared_ptr<VulkanDevice> &pDevice, const RenderEnvInfo &renderEnvInfo);
    ~VulkanPipeline() override;

private:
    std::shared_ptr<VulkanDevice> m_pVulkanDevice;
    std::unique_ptr<VulPipeLineLayout> m_pPipelineLayout;
    std::unique_ptr<VulRenderPass> m_pRenderPass;
    std::unique_ptr<VulPipeline> m_pPipeline;
};

#endif //VULKANPIPELINE_H
