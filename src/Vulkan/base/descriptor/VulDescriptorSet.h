/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/15 14:40
* @version: 1.0
* @description:
********************************************************************************/
#ifndef VULDESCRIPTORSET_H
#define VULDESCRIPTORSET_H

#include "../../../common/common.h"
#include "../VulObject.h"

class VulImage2D;
class VulUniformBuffer;
class VulDescriptorSetLayout;
class VulDescriptorPool;
class VulLogicDevice;

class VulDescriptorSet final : public VulObject<VkDescriptorSet> {
public:
	struct ImageBindingInfo {
		VkDescriptorImageInfo* imageInfo;
		uint32_t               binding;
	};

public:
	VulDescriptorSet(VulLogicDevice* device, VulDescriptorPool* pool, VulDescriptorSetLayout* layout);
	~VulDescriptorSet() override;
	void SetImage(const std::vector<ImageBindingInfo>& vecImageInfo) const;
	void SetUniformBuffer(VulUniformBuffer** buffers, uint32_t count) const;

private:
	VulLogicDevice* m_pLogicDevice = nullptr;
};

#endif // VULDESCRIPTORSET_H
