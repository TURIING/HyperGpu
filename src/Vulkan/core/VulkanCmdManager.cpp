/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/9 20:55
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulkanCmdManager.h"

#include "../base/device/VulPhysicalDevice.h"
#include "../base/command/VulCommandPool.h"
#include "../base/command/VulCommandBuffer.h"
#include "VulkanCmd.h"
#include "VulkanDevice.h"

VulkanCmdManager::VulkanCmdManager(VulkanDevice* device) : m_pVulkanDevice(device) {
	m_pVulkanDevice->AddRef();
	m_pCmdPool = new VulCommandPool(m_pVulkanDevice->GetLogicDevice(), m_pVulkanDevice->GetPhysicalDevice()->GetQueueFamily(QueueType::Graphics));
	m_pQueue = new VulQueue(m_pVulkanDevice->GetLogicDevice(), m_pVulkanDevice->GetPhysicalDevice()->GetQueueFamily(QueueType::Graphics));
}

VulkanCmdManager::~VulkanCmdManager() {
	m_pVulkanDevice->SubRef();
	m_pCmdPool->SubRef();
	m_pQueue->SubRef();
}

GpuCmd* VulkanCmdManager::CreateCommandBuffer() {
	return new VulkanCmd(m_pVulkanDevice, m_pCmdPool);
}

void VulkanCmdManager::WithSingleCmdBuffer(const std::function<void(VulCommandBuffer* cmd)>& func) const {
	const auto vulkanCmd = new VulkanCmd(m_pVulkanDevice, m_pCmdPool);
	const auto cmd       = vulkanCmd->GetHandle();
	cmd->BeginRecord(true);
	func(cmd);
	cmd->EndRecord();

	auto pCmdHandle = cmd->GetHandle();
	m_pQueue->Submit({
		.cmdCount = 1,
		.pCmd = &pCmdHandle,
	});
	m_pQueue->Wait();

	vulkanCmd->SubRef();
}

void VulkanCmdManager::WithSingleCmdBuffer(const std::function<void(GpuCmd*)>& func) {
	const auto vulkanCmd = new VulkanCmd(m_pVulkanDevice, m_pCmdPool);
	const auto pVulCmd   = vulkanCmd->GetHandle();
	pVulCmd->BeginRecord(true);
	func(vulkanCmd);
	pVulCmd->EndRecord();

	auto pCmdHandle = pVulCmd->GetHandle();
	m_pQueue->Submit({
		.cmdCount = 1,
		.pCmd = &pCmdHandle,
	});
	m_pQueue->Wait();

	vulkanCmd->SubRef();
}
