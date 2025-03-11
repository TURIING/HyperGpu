/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 15:58
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulLogicDevice.h"

#include "VulCommandBuffer.h"
#include "VulInstance.h"
#include "VulPhysicalDevice.h"
#include "VulQueue.h"
#include "VulSurface.h"
#include "../core/VulkanCmdManager.h"


VulLogicDevice::VulLogicDevice(const std::shared_ptr<VulInstance> &pInstance, const std::shared_ptr<VulPhysicalDevice> &pPhysicalDevice, const std::shared_ptr<VulSurface>& surface, const VulLogicDeviceCreateInfo &info)
    : m_pInstance(pInstance), m_pPhysicalDevice(pPhysicalDevice), m_pSurface(surface) {
    const auto indices = pPhysicalDevice->GetQueueFamilyIndices();
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    float queuePriority = 1.0f;
    for(const auto queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .pNext = nullptr,
            .queueFamilyIndex = queueFamily,
            .queueCount = 1,
            .pQueuePriorities = &queuePriority,
        };
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkDeviceCreateInfo createInfo {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = nullptr,
        .queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()),
        .pQueueCreateInfos = queueCreateInfos.data(),
        .enabledExtensionCount = static_cast<uint32_t>(info.extensions.size()),
        .ppEnabledExtensionNames = info.extensions.data(),
        .pEnabledFeatures = &info.deviceFeatures,
    };
    if (m_pInstance->IsEnableValidationLayers()) {
        createInfo.enabledLayerCount = 1;
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
}

void VulLogicDevice::WithSingleCmdBuffer(const std::function<void(const std::shared_ptr<VulCommandBuffer>& cmd)>& func) const {
    const auto cmd = std::dynamic_pointer_cast<VulCommandBuffer>(m_pCmdManager->CreateCommandBuffer());
    cmd->BeginRecord();
    func(cmd);
    cmd->EndRecord();
    cmd->Submit();
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

std::shared_ptr<VulLogicDevice> VulLogicDeviceBuilder::Build() {
    return std::make_shared<VulLogicDevice>(m_pInstance, m_pPhysicalDevice, m_pSurface, m_createInfo);
}

