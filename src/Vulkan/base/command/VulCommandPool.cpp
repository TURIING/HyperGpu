/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/8 15:46
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulCommandPool.h"
#include "../device/VulLogicDevice.h"

VulCommandPool::VulCommandPool(VulLogicDevice* device, uint32_t queueFamilyIndex) : m_pDevice(device) {
	m_pDevice->AddRef();

	VkCommandPoolCreateInfo poolInfo = {
		.sType			  = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.flags			  = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		.queueFamilyIndex = queueFamilyIndex,
	};
	CALL_VK(vkCreateCommandPool(m_pDevice->GetHandle(), &poolInfo, nullptr, &m_pHandle));
	LOG_INFO("Command pool created");
}

VulCommandPool::~VulCommandPool() {
	vkDestroyCommandPool(m_pDevice->GetHandle(), m_pHandle, nullptr);
	m_pDevice->SubRef();
}
