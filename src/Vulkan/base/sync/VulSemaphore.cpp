/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/11 21:35
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulSemaphore.h"
#include "../device/VulLogicDevice.h"

USING_GPU_NAMESPACE_BEGIN

VulSemaphore::VulSemaphore(VulLogicDevice* device) : m_pLogicDevice(device) {
	m_pLogicDevice->AddRef();
	VkSemaphoreCreateInfo semaphoreInfo = {
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
	};
	CALL_VK(vkCreateSemaphore(m_pLogicDevice->GetHandle(), &semaphoreInfo, nullptr, &m_pHandle));
	LOG_INFO("Semaphore created");
}

VulSemaphore::~VulSemaphore() {
	LOG_ASSERT(m_pHandle);
	vkDestroySemaphore(m_pLogicDevice->GetHandle(), m_pHandle, nullptr);
	m_pLogicDevice->SubRef();
}

USING_GPU_NAMESPACE_END