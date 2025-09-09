//
// Created by turiing on 2025/9/9.
//

#ifndef VULKANCOMPUTEPIPELINE_H
#define VULKANCOMPUTEPIPELINE_H

#include "../../../common/common.h"
#include "VulkanPipeline.h"

USING_GPU_NAMESPACE_BEGIN

class VulkanComputePipeline final: public VulkanPipeline {
public:
    VulkanComputePipeline(VulkanDevice* pDevice, const ComputeEnvInfo& computeEnvInfo);
    ~VulkanComputePipeline() override;
};

USING_GPU_NAMESPACE_END

#endif //VULKANCOMPUTEPIPELINE_H
