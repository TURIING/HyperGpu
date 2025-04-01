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

void VulkanFence::Wait() const {
	m_pFence->Wait();
}

void VulkanFence::Reset() const {
	m_pFence->Reset();
}
