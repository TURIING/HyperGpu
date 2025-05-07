/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/15 14:40
 * @version: 1.0
 * @description:
 ********************************************************************************/
#include "VulDescriptorSet.h"

#include "../device/VulLogicDevice.h"
#include "VulDescriptorPool.h"
#include "VulDescriptorSetLayout.h"
#include "../resource/VulUniformBuffer.h"

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
}

void VulDescriptorSet::SetImage(const std::vector<ImageBindingInfo>& vecImageInfo) const {
	std::vector<VkWriteDescriptorSet> vecWriteDescriptorSet;
	vecWriteDescriptorSet.reserve(vecImageInfo.size());
	for (const auto& imageBindingInfo : vecImageInfo) {
		vecWriteDescriptorSet.push_back({
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.dstSet = m_pHandle,
			.dstBinding = imageBindingInfo.binding,
			.dstArrayElement = 0,
			.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
			.descriptorCount = 1,
			.pImageInfo = imageBindingInfo.imageInfo,
		});
	}
	vkUpdateDescriptorSets(m_pLogicDevice->GetHandle(), vecWriteDescriptorSet.size(), vecWriteDescriptorSet.data(), 0,
	                       nullptr);
}

void VulDescriptorSet::SetUniformBuffer(VulUniformBuffer** buffers, uint32_t count) const {
	std::vector<VkWriteDescriptorSet> vecWriteDescriptorSet;

	for (auto i = 0; i < count; i++) {
		const auto bufferInfo = buffers[i]->GetDescriptorBufferInfo();

		vecWriteDescriptorSet.push_back({
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.dstSet = m_pHandle,
			.dstBinding = buffers[i]->GetBinding(),
			.dstArrayElement = 0,
			.descriptorCount = 1,
			.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			.pBufferInfo = &bufferInfo,
		});
	}
	vkUpdateDescriptorSets(m_pLogicDevice->GetHandle(), vecWriteDescriptorSet.size(), vecWriteDescriptorSet.data(), 0,
	                       nullptr);
}
