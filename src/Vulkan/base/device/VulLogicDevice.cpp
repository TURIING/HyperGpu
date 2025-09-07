/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 15:58
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulLogicDevice.h"

#include "../../core/VulkanCmd.h"
#include "../../core/VulkanCmdManager.h"
#include "VulInstance.h"
#include "VulPhysicalDevice.h"
#include "VulQueue.h"
#include "../command/VulCommandBuffer.h"
#include "../surface/VulSurface.h"

USING_GPU_NAMESPACE_BEGIN

VulLogicDevice::VulLogicDevice(VulInstance* pInstance, VulPhysicalDevice* pPhysicalDevice, VulSurface* surface, const VulLogicDeviceCreateInfo& info)
	: m_pInstance(pInstance), m_pPhysicalDevice(pPhysicalDevice) {
	m_pInstance->AddRef();
	m_pPhysicalDevice->AddRef();

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	queueCreateInfos.reserve(info.queueInfoCount);

	for(auto i = 0; i < info.queueInfoCount; i++) {
		auto queueFamilyIndex = m_pPhysicalDevice->GetQueueFamily(info.pQueueInfo[i].type);
		auto it = std::find_if(queueCreateInfos.begin(), queueCreateInfos.end(),  [queueFamilyIndex](const VkDeviceQueueCreateInfo& info) {
			if (info.queueFamilyIndex != queueFamilyIndex) {
				return false;
			}
			else {
				return true;
			}
		});
		if (it != queueCreateInfos.end()) continue;

		VkDeviceQueueCreateInfo queueCreateInfo{
			.sType			  = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.pNext			  = nullptr,
			.queueFamilyIndex = queueFamilyIndex,
			.queueCount		  = 1,
			.pQueuePriorities = &info.pQueueInfo[i].priority,
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
}

VulLogicDevice::~VulLogicDevice() {
	vkDestroyDevice(m_pHandle, nullptr);
	m_pInstance->SubRef();
	m_pPhysicalDevice->SubRef();
}

void VulLogicDevice::SetCmdManager(VulkanCmdManager* pCmdManager) {
	m_pCmdManager = pCmdManager;
	m_pCmdManager->AddRef();
}

void VulLogicDevice::WithSingleCmdBuffer(const std::function<void(VulCommandBuffer* cmd)>& func) const {
	m_pCmdManager->WithSingleCmdBuffer(func);
}

void VulLogicDevice::SetDebugUtilsObjectName(VkObjectType type, uint64_t handle, const char *name) const {
	VkDebugUtilsObjectNameInfoEXT nameInfo{
		.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
		.pNext = nullptr,
		.objectType = type,
		.objectHandle = handle,
		.pObjectName = name,
	};
	this->vkSetDebugUtilsObjectNameExt(&nameInfo);
}

void VulLogicDevice::vkSetDebugUtilsObjectNameExt(const VkDebugUtilsObjectNameInfoEXT* pNameInfo) const {
	auto func = (PFN_vkSetDebugUtilsObjectNameEXT) vkGetInstanceProcAddr(m_pInstance->GetHandle(), "vkSetDebugUtilsObjectNameEXT");
	if (func != nullptr) {
		if (func(m_pHandle, pNameInfo) != VK_SUCCESS) {
			LOG_CRITICAL("Set debug utils name failed");
		}
	} else {
		LOG_CRITICAL("Load vulkan func failed.");
	}
}

VulLogicDeviceBuilder VulLogicDevice::Builder() {
    return VulLogicDeviceBuilder{ };
}

VulLogicDeviceBuilder& VulLogicDeviceBuilder::AddExtension(const char* ext) {
    m_createInfo.extensions.push_back(ext);
    return *this;
}

VulLogicDeviceBuilder& VulLogicDeviceBuilder::SetQueueInfos(HyperGpu::QueueInfo* pQueueInfo, u32 count) {
	m_createInfo.pQueueInfo = pQueueInfo;
	m_createInfo.queueInfoCount = count;
	return *this;
}

VulLogicDevice* VulLogicDeviceBuilder::Build() const {
	return new VulLogicDevice(m_pInstance, m_pPhysicalDevice, m_pSurface, m_createInfo);
}

USING_GPU_NAMESPACE_END