/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/11 21:35
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULFENCE_H
#define VULFENCE_H
#include "../VulObject.h"
#include "../../../common/common.h"

class VulLogicDevice;

class VulFence final : public VulObject<VkFence>{
public:
	explicit VulFence(VulLogicDevice* device);
	~		 VulFence() override;
	WaitState Wait(u32 timeout) const;
	void	 Reset() const;

private:
	VulLogicDevice* m_pLogicDevice = nullptr;
};

#endif //VULFENCE_H
