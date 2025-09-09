/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 23:26
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULKANPIPELINEMANAGER_H
#define VULKANPIPELINEMANAGER_H

#include "../../../common/common.h"

USING_GPU_NAMESPACE_BEGIN

class VulkanDevice;

class VulkanPipelineManager final : public PipelineManager {
public:
	explicit VulkanPipelineManager(VulkanDevice* device);
	~VulkanPipelineManager() override;
	NODISCARD Pipeline* CreateGraphicPipeline(const RenderEnvInfo& renderEnvInfo) override;
	NODISCARD Pipeline * CreateComputePipeline(const ComputeEnvInfo& computeEnvInfo) override;

private:
	VulkanDevice* m_pDevice = nullptr;
};

USING_GPU_NAMESPACE_END
#endif //VULKANPIPELINEMANAGER_H
