/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/11 21:35
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULSEMAPHORE_H
#define VULSEMAPHORE_H

#include "../../../common/common.h"
#include "../VulObject.h"


class VulLogicDevice;

class VulSemaphore final : public VulObject<VkSemaphore>{
public:
	explicit VulSemaphore(VulLogicDevice* device);
	~		 VulSemaphore() override;

private:
	VulLogicDevice* m_pLogicDevice = nullptr;
};

#endif //VULSEMAPHORE_H
