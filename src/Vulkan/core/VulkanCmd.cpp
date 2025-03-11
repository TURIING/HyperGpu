/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/8 15:33
* @version: 1.0
* @description: 
********************************************************************************/
#include "../../common/common.h"
#include "VulkanCmd.h"

#include "VulkanDevice.h"
#include "../base/VulCommandPool.h"
#include "../base/VulPhysicalDevice.h"

VulkanCmd::VulkanCmd(const std::shared_ptr<VulkanDevice>& device, const std::shared_ptr<VulCommandPool>& pool) : m_pVulkanDevice(device) {
}
