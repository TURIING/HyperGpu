/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 15:58
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULLOGICDEVICE_H
#define VULLOGICDEVICE_H

#include <GpuDevice.h>

#include "../VulObject.h"
#include "../../../common/common.h"

class VulCommandBuffer;
class VulkanCmdManager;
class VulLogicDevice;
class VulSurface;
class VulPhysicalDevice;
class VulInstance;

struct VulLogicDeviceCreateInfo {
    std::vector<const char*> extensions;
    VkPhysicalDeviceFeatures deviceFeatures;
	HyperGpu::QueueInfo* pQueueInfo = nullptr;
	u32 queueInfoCount = 0;
};

class VulLogicDeviceBuilder {
public:
	VulLogicDeviceBuilder& SetInstance(VulInstance* instance) { m_pInstance = instance; return *this; }
	VulLogicDeviceBuilder& SetPhysicalDevice(VulPhysicalDevice* physicalDevice) { m_pPhysicalDevice = physicalDevice; return *this; }
	VulLogicDeviceBuilder& AddExtension(const char* ext);
	VulLogicDeviceBuilder& SetDeviceFeatures(const VkPhysicalDeviceFeatures& deviceFeatures) { m_createInfo.deviceFeatures = deviceFeatures; return *this; }
	VulLogicDeviceBuilder& SetQueueInfos(QueueInfo* pQueueInfo, u32 count);
	[[nodiscard]] VulLogicDevice* Build() const;

private:
	VulInstance*			 m_pInstance	   = nullptr;
	VulPhysicalDevice*		 m_pPhysicalDevice = nullptr;
	VulSurface*				 m_pSurface		   = nullptr;
	VulLogicDeviceCreateInfo m_createInfo{};
};

class VulLogicDevice final : public VulObject<VkDevice>{
public:
	 VulLogicDevice(VulInstance* pInstance, VulPhysicalDevice* pPhysicalDevice, VulSurface* surface, const VulLogicDeviceCreateInfo& info);
	~VulLogicDevice() override;
	[[nodiscard]] VulPhysicalDevice* GetPhysicalDevice() const { return m_pPhysicalDevice; }
	void SetCmdManager(VulkanCmdManager* pCmdManager);
	void WithSingleCmdBuffer(const std::function<void(VulCommandBuffer* cmd)>& func) const;
	static VulLogicDeviceBuilder Builder();

private:
	VulInstance*	   m_pInstance		 = nullptr;
	VulPhysicalDevice* m_pPhysicalDevice = nullptr;
	VulkanCmdManager*  m_pCmdManager	 = nullptr;
};

#endif // VULLOGICDEVICE_H
