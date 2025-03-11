/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/9 20:55
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULKANCMDMANAGER_H
#define VULKANCMDMANAGER_H

#include <GpuCmd.h>

#include "../../common/common.h"

class VulkanDevice;
class VulCommandPool;

class VulkanCmdManager final : public GpuCmdManager{
public:
    explicit VulkanCmdManager(const std::shared_ptr<VulkanDevice>& device);
    ~VulkanCmdManager() override;
    [[nodiscard]] std::shared_ptr<GpuCmd> CreateCommandBuffer() override;

private:
    std::shared_ptr<VulkanDevice> m_pVulkanDevice;
    std::shared_ptr<VulCommandPool> m_pCmdPool;

};



#endif //VULKANCMDMANAGER_H
