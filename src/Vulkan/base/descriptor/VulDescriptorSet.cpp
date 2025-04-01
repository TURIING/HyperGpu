/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/15 14:40
 * @version: 1.0
 * @description:
 ********************************************************************************/
#include "VulDescriptorSet.h"

#include "../VulLogicDevice.h"
#include "VulDescriptorPool.h"
#include "VulDescriptorSetLayout.h"

VulDescriptorSet::VulDescriptorSet(VulLogicDevice* device, VulDescriptorPool* pool, VulDescriptorSetLayout* layout) : m_pLogicDevice(device) {
	m_pLogicDevice->AddRef();
	std::vector<VkDescriptorSetLayout> descriptorSetLayouts = {layout->GetHandle()};

	const VkDescriptorSetAllocateInfo allocateInfo{
		.sType				= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		.descriptorPool		= pool->GetHandle(),
		.descriptorSetCount = static_cast<uint32_t>(descriptorSetLayouts.size()),
		.pSetLayouts		= descriptorSetLayouts.data(),
	};
	CALL_VK(vkAllocateDescriptorSets(m_pLogicDevice->GetHandle(), &allocateInfo, &m_pHandle));
	LOG_INFO("Descriptor Set Allocated");
}

VulDescriptorSet::~VulDescriptorSet() {
	m_pLogicDevice->SubRef();
};

void VulDescriptorSet::SetImage(const VkDescriptorImageInfo& imageInfo, uint32_t binding) const {
	const VkWriteDescriptorSet writeDescriptorSet{
		.sType			 = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
		.dstSet			 = m_pHandle,
		.dstBinding		 = binding,
		.dstArrayElement = 0,
		.descriptorType	 = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		.descriptorCount = 1,
		.pImageInfo		 = &imageInfo,
	};
	vkUpdateDescriptorSets(m_pLogicDevice->GetHandle(), 1, &writeDescriptorSet, 0, nullptr);
}
