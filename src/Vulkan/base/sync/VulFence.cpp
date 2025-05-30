/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/11 21:35
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulFence.h"
#include "../device/VulLogicDevice.h"

VulFence::VulFence(VulLogicDevice* device) : m_pLogicDevice(device) {
	m_pLogicDevice->AddRef();
	VkFenceCreateInfo fenceCreateInfo = {
		.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
		.flags = VK_FENCE_CREATE_SIGNALED_BIT, // 用于指定围栏在创建时的初始状态为“已触发”（signaled）
	};
	vkCreateFence(m_pLogicDevice->GetHandle(), &fenceCreateInfo, nullptr, &m_pHandle);
	LOG_INFO("Fence created");
}

VulFence::~VulFence() {
	vkDestroyFence(m_pLogicDevice->GetHandle(), m_pHandle, nullptr);
	m_pLogicDevice->SubRef();
}

void VulFence::Wait() const {
	vkWaitForFences(m_pLogicDevice->GetHandle(), 1, &m_pHandle, VK_TRUE, UINT64_MAX);
}

void VulFence::Reset() const {
	vkResetFences(m_pLogicDevice->GetHandle(), 1, &m_pHandle);
}