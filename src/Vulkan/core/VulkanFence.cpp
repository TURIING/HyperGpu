/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/16 16:45
 * @version: 1.0
 * @description:
 ********************************************************************************/
#include "VulkanFence.h"

#include "../base/sync/VulFence.h"
#include "VulkanDevice.h"

VulkanFence::VulkanFence(const VulkanDevice* device) {
	m_pFence = new VulFence(device->GetLogicDevice());
}

VulkanFence::~VulkanFence() {
	m_pFence->SubRef();
};

WaitState VulkanFence::Wait(uint32_t timeout) {
	return m_pFence->Wait(timeout);
}

void VulkanFence::Reset() {
	m_pFence->Reset();
}
