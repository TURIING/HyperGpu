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

class VulkanPipeline final : public Pipeline {
public:
	VulkanPipeline(VulkanDevice* pDevice, const RenderEnvInfo& renderEnvInfo);
	~VulkanPipeline() override;
	NODISCARD VulPipeline*       GetHandle() const { return m_pPipeline; }
	NODISCARD VulRenderPass*     GetRenderPass() const { return m_pRenderPass; }
	NODISCARD VulPipeLineLayout* GetPipelineLayout() const { return m_pPipelineLayout; }
	NODISCARD VulDescriptorSetLayout* GetDescriptorSetLayout() const { return m_pDescriptorSetLayout; }
	NODISCARD VulShader* GetShader() const { return m_pShader; }

private:
	VulkanDevice*           m_pVulkanDevice        = nullptr;
	VulPipeLineLayout*      m_pPipelineLayout      = nullptr;
	VulRenderPass*          m_pRenderPass          = nullptr;
	VulPipeline*            m_pPipeline            = nullptr;
	VulDescriptorPool*      m_pDescriptorPool      = nullptr;
	VulDescriptorSetLayout* m_pDescriptorSetLayout = nullptr;
	VulShader*              m_pShader		       = nullptr;
};

USING_GPU_NAMESPACE_END
#endif // VULKANPIPELINE_H