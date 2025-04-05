/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 15:58
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulLogicDevice.h"

#include "../core/VulkanCmd.h"
#include "../core/VulkanCmdManager.h"
#include "VulInstance.h"
#include "VulPhysicalDevice.h"
#include "VulQueue.h"
#include "command/VulCommandBuffer.h"
#include "surface/VulSurface.h"

VulLogicDevice::VulLogicDevice(VulInstance* pInstance, VulPhysicalDevice* pPhysicalDevice, VulSurface* surface, const VulLogicDeviceCreateInfo& info)
	: m_pInstance(pInstance), m_pPhysicalDevice(pPhysicalDevice), m_pSurface(surface) {
	m_pInstance->AddRef();
	m_pPhysicalDevice->AddRef();
	m_pSurface->AddRef();

	const auto							 indices = pPhysicalDevice->GetQueueFamilyIndices();
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t>					 uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

	float queuePriority = 1.0f;
	for(const auto queueFamily : uniqueQueueFamilies) {
		VkDeviceQueueCreateInfo queueCreateInfo{
			.sType			  = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.pNext			  = nullptr,
			.queueFamilyIndex = queueFamily,
			.queueCount		  = 1,
			.pQueuePriorities = &queuePriority,
		};
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkDeviceCreateInfo createInfo{
		.sType					 = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pNext					 = nullptr,
		.queueCreateInfoCount	 = static_cast<uint32_t>(queueCreateInfos.size()),
		.pQueueCreateInfos		 = queueCreateInfos.data(),
		.enabledExtensionCount	 = static_cast<uint32_t>(info.extensions.size()),
		.ppEnabledExtensionNames = info.extensions.data(),
		.pEnabledFeatures		 = &info.deviceFeatures,
	};
	if(m_pInstance->IsEnableValidationLayers()) {
		createInfo.enabledLayerCount   = 1;
		createInfo.ppEnabledLayerNames = &VK_LAYER_KHRONOS_VALIDATION;
	}
	CALL_VK(vkCreateDevice(m_pPhysicalDevice->GetHandle(), &createInfo, nullptr, &m_pHandle));
	LOG_INFO("Logical Device created!");

	if(this->checkPresentSupport(indices.graphicsFamily.value())) {
		vkGetDeviceQueue(m_pHandle, indices.graphicsFamily.value(), 0, &m_pGraphicsQueue);
	}
	if(this->checkPresentSupport(indices.presentFamily.value())) {
		vkGetDeviceQueue(m_pHandle, indices.presentFamily.value(), 0, &m_pPresentQueue);
	}
}

VulLogicDevice::~VulLogicDevice() {
	vkDestroyDevice(m_pHandle, nullptr);
	m_pInstance->SubRef();
	m_pSurface->SubRef();
	m_pPhysicalDevice->SubRef();
}

void VulLogicDevice::SetCmdManager(VulkanCmdManager* pCmdManager) {
	m_pCmdManager = pCmdManager;
	m_pCmdManager->AddRef();
}

void VulLogicDevice::WithSingleCmdBuffer(const std::function<void(VulCommandBuffer* cmd)>& func) const {
	m_pCmdManager->WithSingleCmdBuffer(func);
}

VulLogicDeviceBuilder VulLogicDevice::Builder() {
    return VulLogicDeviceBuilder{ };
}


bool VulLogicDevice::checkPresentSupport(uint32_t queueFamilyIndex) const {
    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(m_pPhysicalDevice->GetHandle(), queueFamilyIndex, m_pSurface->GetHandle(), &presentSupport);
    return presentSupport;
}

VulLogicDeviceBuilder& VulLogicDeviceBuilder::AddExtension(const char* ext) {
    m_createInfo.extensions.push_back(ext);
    return *this;
}

VulLogicDevice* VulLogicDeviceBuilder::Build() const {
	return new VulLogicDevice(m_pInstance, m_pPhysicalDevice, m_pSurface, m_createInfo);
}
