/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/1 23:00
 * @version: 1.0
 * @description:
 ********************************************************************************/
#ifndef VULKANPIPELINE_H
#define VULKANPIPELINE_H

#include "../../../common/common.h"

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

class VulkanPipeline: public Pipeline {
public:
	VulkanPipeline(VulkanDevice* pDevice, const EnvInfo& envInfo);
	~VulkanPipeline() override;
	NODISCARD VulPipeline* GetHandle() const { return m_pPipeline; }
	NODISCARD VulPipeLineLayout* GetPipelineLayout() const { return m_pPipelineLayout; }
	NODISCARD VulDescriptorSetLayout* GetDescriptorSetLayout() const { return m_pDescriptorSetLayout; }
	NODISCARD VulShader* GetShader() const { return m_pShader; }

protected:
	VulkanDevice*           m_pVulkanDevice        = nullptr;
	VulPipeLineLayout*      m_pPipelineLayout      = nullptr;
	VulPipeline*            m_pPipeline            = nullptr;
	VulDescriptorSetLayout* m_pDescriptorSetLayout = nullptr;
	VulShader*              m_pShader		       = nullptr;
};

USING_GPU_NAMESPACE_END
#endif // VULKANPIPELINE_H