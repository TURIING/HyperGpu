/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/8 15:46
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULCOMMANDPOOL_H
#define VULCOMMANDPOOL_H

#include "VulObject.h"
#include "../../common/common.h"

class VulLogicDevice;

class VulCommandPool final : public VulObject<VkCommandPool>{
public:
    VulCommandPool(const std::shared_ptr<VulLogicDevice>& device, uint32_t queueFamilyIndex);

private:
    std::shared_ptr<VulLogicDevice> m_pDevice;
};



#endif //VULCOMMANDPOOL_H
