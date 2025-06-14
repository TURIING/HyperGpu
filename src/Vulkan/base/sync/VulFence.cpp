/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/11 21:35
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulFence.h"
#include "../device/VulLogicDevice.h"

USING_GPU_NAMESPACE_BEGIN

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

WaitState VulFence::Wait(u32 timeout) const {
	const auto result = vkWaitForFences(m_pLogicDevice->GetHandle(), 1, &m_pHandle, VK_TRUE, timeout);
	if (result == VK_TIMEOUT) {
		return WaitState::Timeout;
	}
	else if (result == VK_SUCCESS) {
		return WaitState::Success;
	}
	return WaitState::Failed;
}

void VulFence::Reset() const {
    CALL_VK(vkResetFences(m_pLogicDevice->GetHandle(), 1, &m_pHandle));
}

USING_GPU_NAMESPACE_END