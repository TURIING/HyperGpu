/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/11 21:35
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULFENCE_H
#define VULFENCE_H
#include <memory>
#include <vulkan/vulkan_core.h>

#include "../VulObject.h"


class VulLogicDevice;

class VulFence final : public VulObject<VkFence>{
public:
	explicit VulFence(VulLogicDevice* device);
	~		 VulFence() override;
	void	 Wait() const;
	void	 Reset() const;

private:
	VulLogicDevice* m_pLogicDevice = nullptr;
};

#endif //VULFENCE_H
