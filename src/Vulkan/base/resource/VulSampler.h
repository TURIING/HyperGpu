/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/15 13:39
 * @version: 1.0
 * @description:
 ********************************************************************************/
#ifndef VULSAMPLER_H
#define VULSAMPLER_H

#include "../../../common/common.h"
#include "../VulObject.h"

class VulLogicDevice;

struct VulSamplerCreateInfo {
	VkFilter             magFilter;
	VkFilter             minFilter;
	VkSamplerAddressMode addressModeU;
	VkSamplerAddressMode addressModeV;
	VkSamplerAddressMode addressModeW;
};

class VulSampler final : public VulObject<VkSampler> {
public:
	VulSampler(VulLogicDevice* device, const VulSamplerCreateInfo& info);
	~VulSampler() override;

private:
	VulLogicDevice* m_pLogicDevice = nullptr;
};

#endif // VULSAMPLER_H
