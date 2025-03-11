/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/8 15:34
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULKANCMD_H
#define VULKANCMD_H
#include <GpuCmd.h>
#include <memory>

class VulCommandPool;
class VulkanDevice;

class VulkanCmd final : public GpuCmd{
public:
    VulkanCmd(const std::shared_ptr<VulkanDevice>& device, const std::shared_ptr<VulCommandPool>& pool);

private:
    std::shared_ptr<VulkanDevice> m_pVulkanDevice;
};

#endif //VULKANCMD_H
