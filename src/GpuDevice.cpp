/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/2/22 00:29
* @version: 1.0
* @description: 
********************************************************************************/

#include "GpuDevice.h"
#include "OpenGL/core/OpenGlDevice.h"
#include "Vulkan/core/VulkanDevice.h"

GpuFactory::GpuFactory(GpuType type): m_type(type) {

}

std::unique_ptr<GpuDevice> GpuFactory::CreateDevice(const DeviceCreateInfo &info) const {
    switch (m_type) {
        case GpuType::OPENGL: return std::make_unique<OpenGlDevice>(info);
        case GpuType::VULKAN: return std::make_unique<VulkanDevice>(info);
        default:              LOG_ASSERT(false);
    }
}
