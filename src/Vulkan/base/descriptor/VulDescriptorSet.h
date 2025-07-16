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

USING_GPU_NAMESPACE_BEGIN

class VulImage2D;
class VulUniformBuffer;
class VulDescriptorSetLayout;
class VulDescriptorPool;
class VulLogicDevice;

class VulDescriptorSet final : public VulObject<VkDescriptorSet> {
public:
	struct ImageBindingInfo {
		std::vector<VkDescriptorImageInfo> vecImageInfo;
        std::string name;
	};

	struct UniformBindingInfo {
		VkDescriptorBufferInfo* pBufferInfo = nullptr;
		std::string name;
	};

public:
	VulDescriptorSet(VulLogicDevice* device, VulDescriptorPool* pool, VulDescriptorSetLayout* layout, std::unordered_map<std::string, uint8_t> resourceBinding);
	~VulDescriptorSet() override;
	void SetImage(const std::vector<ImageBindingInfo>& vecImageInfo);
	void SetUniformBuffer(const std::vector<UniformBindingInfo> &vecBufferInfo);

private:
	VulLogicDevice* m_pLogicDevice = nullptr;
    std::unordered_map<std::string, uint8_t> m_mapResourceBinding;
};

USING_GPU_NAMESPACE_END
#endif // VULDESCRIPTORSET_H
