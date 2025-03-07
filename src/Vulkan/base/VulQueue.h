/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/2/24 22:58
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULQUEUE_H
#define VULQUEUE_H

#include "../../common/common.h"

struct QueueFamilyIndices{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    [[nodiscard]] bool isComplete() const { return graphicsFamily.has_value() && presentFamily.has_value(); }
    static QueueFamilyIndices GetQueueFamilyIndices(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
};

class VulQueue {

};

#endif //VULQUEUE_H
