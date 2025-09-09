//
// Created by turiing on 2025/9/8.
//

#ifndef VULKANGRAPHICPIPELINE_H
#define VULKANGRAPHICPIPELINE_H


#include "../../../common/common.h"
#include "VulkanPipeline.h"

USING_GPU_NAMESPACE_BEGIN

class VulDescriptorSetLayout;
class VulIndexBuffer;
class VulVertexBuffer;
class VulDescriptorPool;
class VulDescriptorSet;
class VulPipeline;
class VulRenderPass;
class VulPipeLineLayout;
class VulkanDevice;
class VulShader;

class VulkanGraphicPipeline final : public VulkanPipeline {
public:
    VulkanGraphicPipeline(VulkanDevice* pDevice, const RenderEnvInfo& renderEnvInfo);
    ~VulkanGraphicPipeline() override;
    NODISCARD VulRenderPass* GetRenderPass() const { return m_pRenderPass; }

private:
    VulRenderPass* m_pRenderPass = nullptr;
};

USING_GPU_NAMESPACE_END

#endif //VULKANGRAPHICPIPELINE_H
