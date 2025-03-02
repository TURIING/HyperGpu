/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 23:00
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULKANPIPELINE_H
#define VULKANPIPELINE_H

#include "common/common.h"
#include "core/GpuPipeline.h"

class VulkanDevice;

class VulkanPipeline final : public Pipeline {
public:
    VulkanPipeline(const std::shared_ptr<VulkanDevice> &m_pDevice, const RenderEnvInfo &renderEnvInfo);
};

#endif //VULKANPIPELINE_H
