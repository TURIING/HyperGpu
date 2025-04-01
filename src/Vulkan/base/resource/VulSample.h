/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/15 13:39
 * @version: 1.0
 * @description:
 ********************************************************************************/
#ifndef VULSAMPLE_H
#define VULSAMPLE_H

#include "../../../common/common.h"
#include "../VulObject.h"

class VulLogicDevice;

struct VulSampleCreateInfo {
	VkFilter			 magFilter;
	VkFilter			 minFilter;
	VkSamplerAddressMode addressMode;
};

class VulSample final : public VulObject<VkSampler> {
public:
	 VulSample(VulLogicDevice* device, const VulSampleCreateInfo& info);
	~VulSample() override;

private:
	VulLogicDevice* m_pLogicDevice = nullptr;
};

#endif // VULSAMPLE_H
