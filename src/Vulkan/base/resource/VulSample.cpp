/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/15 13:39
 * @version: 1.0
 * @description:
 ********************************************************************************/
#include "VulSample.h"

#include "../VulLogicDevice.h"
#include "../VulPhysicalDevice.h"

VulSample::VulSample(VulLogicDevice* device, const VulSampleCreateInfo& info) : m_pLogicDevice(device) {
	m_pLogicDevice->AddRef();
	VkSamplerCreateInfo samplerInfo = {
		.sType					 = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
		.magFilter				 = info.magFilter,
		.minFilter				 = info.minFilter,
		.addressModeU			 = info.addressMode,
		.addressModeV			 = info.addressMode,
		.addressModeW			 = info.addressMode,
		.anisotropyEnable		 = VK_TRUE,
		.maxAnisotropy			 = m_pLogicDevice->GetPhysicalDevice()->GetGpuInfo().limits.maxSamplerAnisotropy,
		.borderColor			 = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
		.unnormalizedCoordinates = VK_FALSE,
		.compareEnable			 = VK_FALSE,
		.compareOp				 = VK_COMPARE_OP_ALWAYS,
		.mipmapMode				 = VK_SAMPLER_MIPMAP_MODE_LINEAR,
		.mipLodBias				 = 0.0f,
		.minLod					 = 0.0f,
		.maxLod					 = 11.0f,
	};
	CALL_VK(vkCreateSampler(m_pLogicDevice->GetHandle(), &samplerInfo, nullptr, &m_pHandle));
	LOG_INFO("Created sampler");
}

VulSample::~VulSample() {
	vkDestroySampler(m_pLogicDevice->GetHandle(), m_pHandle, nullptr);
	m_pLogicDevice->SubRef();
}
