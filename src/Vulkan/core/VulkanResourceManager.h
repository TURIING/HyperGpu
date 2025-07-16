/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/15 13:13
 * @version: 1.0
 * @description:
 ********************************************************************************/
#ifndef VULKANRESOURCEMANAGER_H
#define VULKANRESOURCEMANAGER_H

#include "../../../include/GpuResource.h"
#include "../../common/common.h"
#include "VulkanDevice.h"

USING_GPU_NAMESPACE_BEGIN

class VulkanResourceManager final : public GpuResourceManager {
public:
	explicit VulkanResourceManager(VulkanDevice* device);
	~ VulkanResourceManager() override;
	[[nodiscard]] Image2D* CreateImage2D(const Image2D::Image2DCreateInfo& info) override;
	[[nodiscard]] Buffer* CreateBuffer(const Buffer::BufferCreateInfo& createInfo) override;
	[[nodiscard]] Sampler* CreateSampler(const Sampler::SamplerCreateInfo& info) override;
	[[nodiscard]] InputAssembler* CreateInputAssembler(const InputAssemblerInfo& info) override;
	NODISCARD InputAssembler * CreateInputAssembler(const InstanceInputAssemblerInfo &info) override;

private:
	VulkanDevice* m_pVulkanDevice = nullptr;
};

USING_GPU_NAMESPACE_END
#endif // VULKANRESOURCEMANAGER_H
