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

class VulDescriptorSetLayout;
class VulIndexBuffer;
class VulVertexBuffer;
class VulDescriptorPool;
class VulDescriptorSet;
class VulPipeline;
class VulRenderPass;
using namespace HyperGpu;

class VulPipeLineLayout;
class VulkanDevice;

class VulkanPipeline final : public Pipeline {
public:
	VulkanPipeline(VulkanDevice* pDevice, const RenderEnvInfo& renderEnvInfo);
	~ VulkanPipeline() override;
	[[nodiscard]] VulPipeline*       GetHandle() const { return m_pPipeline; }
	[[nodiscard]] VulRenderPass*     GetRenderPass() const { return m_pRenderPass; }
	[[nodiscard]] VulDescriptorSet*  GetDescriptorSet() const { return m_pDescriptorSet; }
	[[nodiscard]] VulPipeLineLayout* GetPipelineLayout() const { return m_pPipelineLayout; }
	void                             SetUniformBuffers(Buffer** buffer, uint32_t count) override;
	void                             SetImages(ImageBindingInfo* infos, uint32_t count) override;

private:
	VulkanDevice*           m_pVulkanDevice        = nullptr;
	VulPipeLineLayout*      m_pPipelineLayout      = nullptr;
	VulRenderPass*          m_pRenderPass          = nullptr;
	VulPipeline*            m_pPipeline            = nullptr;
	VulDescriptorPool*      m_pDescriptorPool      = nullptr;
	VulDescriptorSet*       m_pDescriptorSet       = nullptr;
	VulDescriptorSetLayout* m_pDescriptorSetLayout = nullptr;
};

#endif // VULKANPIPELINE_H
