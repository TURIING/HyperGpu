/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/2/24 23:16
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULSWAPCHAIN_H
#define VULSWAPCHAIN_H

#include "../../common/common.h"

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities {};
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;

    static SwapChainSupportDetails GetSwapChainSupportDetails(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
};

#endif //VULSWAPCHAIN_H
