/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/15 14:40
 * @version: 1.0
 * @description:
 ********************************************************************************/
#include "VulDescriptorSet.h"

#include <utility>

#include "../device/VulLogicDevice.h"
#include "VulDescriptorPool.h"
#include "VulDescriptorSetLayout.h"

USING_GPU_NAMESPACE_BEGIN

VulDescriptorSet::VulDescriptorSet(VulLogicDevice* device, VulDescriptorPool* pool, VulDescriptorSetLayout* layout, std::unordered_map<std::string, uint8_t> resourceBinding)
: m_pLogicDevice(device), m_mapResourceBinding(std::move(resourceBinding)) {
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

void VulDescriptorSet::SetImage(const std::vector<ImageBindingInfo>& vecImageInfo) {
	std::vector<VkWriteDescriptorSet> vecWriteDescriptorSet;
	vecWriteDescriptorSet.reserve(vecImageInfo.size());
	for (auto& imageBindingInfo : vecImageInfo) {
		VkDescriptorType type;
		if (has_flag(imageBindingInfo.imageUsage, ImageUsageFlags::STORAGE)) {
			type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
		}
		else if (has_flag(imageBindingInfo.imageUsage, ImageUsageFlags::SAMPLED)) {
			type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		}
		else {
			LOG_ASSERT(!"Unexpected condition branches");
		}

		vecWriteDescriptorSet.push_back({
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.dstSet = m_pHandle,
			.dstBinding = m_mapResourceBinding[imageBindingInfo.name],
			.dstArrayElement = 0,
			.descriptorCount = TO_U32(imageBindingInfo.vecImageInfo.size()),
			.descriptorType = type,
			.pImageInfo = imageBindingInfo.vecImageInfo.data(),
		});
	}
	vkUpdateDescriptorSets(m_pLogicDevice->GetHandle(), vecWriteDescriptorSet.size(), vecWriteDescriptorSet.data(), 0, nullptr);
}

void VulDescriptorSet::SetUniformBuffer(const std::vector<UniformBindingInfo> &vecBufferInfo) {
	std::vector<VkWriteDescriptorSet> vecWriteDescriptorSet;
	vecWriteDescriptorSet.reserve(vecBufferInfo.size());

	for (auto &bufferBindingInfo : vecBufferInfo) {
		vecWriteDescriptorSet.push_back({
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.dstSet = m_pHandle,
			.dstBinding = m_mapResourceBinding[bufferBindingInfo.name],
			.dstArrayElement = 0,
			.descriptorCount = 1,
			.descriptorType = bufferBindingInfo.type,
			.pBufferInfo = bufferBindingInfo.pBufferInfo,
		});
	}
	vkUpdateDescriptorSets(m_pLogicDevice->GetHandle(), vecWriteDescriptorSet.size(), vecWriteDescriptorSet.data(), 0, nullptr);
}

USING_GPU_NAMESPACE_END