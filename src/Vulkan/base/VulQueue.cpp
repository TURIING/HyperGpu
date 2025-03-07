/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/2/24 23:02
* @version: 1.0
* @description: 
********************************************************************************/

#include "VulQueue.h"

QueueFamilyIndices QueueFamilyIndices::GetQueueFamilyIndices(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) {
    QueueFamilyIndices indices;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    for(int familyIndex = 0; const auto &queueFamily : queueFamilies) {
        if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = familyIndex;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, familyIndex, surface, &presentSupport);
        if(presentSupport) indices.presentFamily = familyIndex;

        if(indices.isComplete()) break;

        familyIndex++;
    }
    return indices;
}