/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/8 15:46
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULCOMMANDPOOL_H
#define VULCOMMANDPOOL_H

#include "../VulObject.h"
#include "../../../common/common.h"

USING_GPU_NAMESPACE_BEGIN

class VulLogicDevice;

class VulCommandPool final : public VulObject<VkCommandPool>{
public:
    VulCommandPool(VulLogicDevice* device, uint32_t queueFamilyIndex);
	~VulCommandPool() override;

private:
    VulLogicDevice* m_pDevice = nullptr;
};

USING_GPU_NAMESPACE_END
#endif //VULCOMMANDPOOL_H