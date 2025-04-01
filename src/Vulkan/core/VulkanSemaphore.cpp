/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/16 16:45
 * @version: 1.0
 * @description:
 ********************************************************************************/
#include "VulkanSemaphore.h"

#include "../base/sync/VulSemaphore.h"
#include "VulkanDevice.h"

VulkanSemaphore::VulkanSemaphore(VulkanDevice* device) {
	m_pSemaphore = new VulSemaphore(device->GetLogicDevice());
}

VulkanSemaphore::~VulkanSemaphore() {
	m_pSemaphore->SubRef();
}
