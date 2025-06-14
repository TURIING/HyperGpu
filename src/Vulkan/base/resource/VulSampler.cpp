/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/15 13:39
 * @version: 1.0
 * @description:
 ********************************************************************************/
#include "VulSampler.h"

#include "../device/VulLogicDevice.h"
#include "../device/VulPhysicalDevice.h"

USING_GPU_NAMESPACE_BEGIN

VulSampler::VulSampler(VulLogicDevice* device, const VulSamplerCreateInfo& info) : m_pLogicDevice(device) {
	m_pLogicDevice->AddRef();
	VkSamplerCreateInfo samplerInfo = {
		.sType					 = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
		.magFilter				 = info.magFilter,
		.minFilter				 = info.minFilter,
		.mipmapMode				 = VK_SAMPLER_MIPMAP_MODE_LINEAR,
		.addressModeU = info.addressModeU,
		.addressModeV = info.addressModeV,
		.addressModeW = info.addressModeW,
		.mipLodBias				 = 0.0f,
		.anisotropyEnable		 = VK_TRUE,
		.maxAnisotropy			 = m_pLogicDevice->GetPhysicalDevice()->GetGpuInfo().limits.maxSamplerAnisotropy,
		.compareEnable			 = VK_FALSE,
		.compareOp				 = VK_COMPARE_OP_ALWAYS,
		.minLod					 = 0.0f,
		.maxLod					 = 11.0f,
		.borderColor			 = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
		.unnormalizedCoordinates = VK_FALSE,
	};
	CALL_VK(vkCreateSampler(m_pLogicDevice->GetHandle(), &samplerInfo, nullptr, &m_pHandle));
	LOG_INFO("Created sampler");
}

VulSampler::~VulSampler() {
	vkDestroySampler(m_pLogicDevice->GetHandle(), m_pHandle, nullptr);
	m_pLogicDevice->SubRef();
}

USING_GPU_NAMESPACE_END