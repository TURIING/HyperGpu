/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/11 22:42
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulImage2D.h"
#include "../device/VulLogicDevice.h"
#include "../device/VulPhysicalDevice.h"
#include "../command/VulCommandBuffer.h"

USING_GPU_NAMESPACE_BEGIN

VulImage2D::VulImage2D(VulLogicDevice* device, const VulImage2DCreateInfo& info) : m_pLogicDevice(device), m_mipLevels(info.mipLevels){
	m_pLogicDevice->AddRef();

    if (info.handle) {
        m_pHandle = info.handle;
    }
    else {
        const VkImageCreateInfo imageCreateInfo {
            .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
            .imageType = VK_IMAGE_TYPE_2D,
            .format = info.format,
            .extent = {
                .width = info.size.width,
                .height = info.size.height,
                .depth = 1,
            },
            .mipLevels = info.mipLevels,
            .arrayLayers = 1,
            .samples = info.samples,
            .tiling = VK_IMAGE_TILING_OPTIMAL,
            .usage = info.usage,
            .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        };
        CALL_VK(vkCreateImage(m_pLogicDevice->GetHandle(), &imageCreateInfo, nullptr, &m_pHandle));

        // 分配图像内存
        VkMemoryRequirements memoryRequirements;
        vkGetImageMemoryRequirements(m_pLogicDevice->GetHandle(), m_pHandle, &memoryRequirements);

        VkMemoryAllocateInfo memoryAllocateInfo {
            .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            .allocationSize = memoryRequirements.size,
            .memoryTypeIndex = m_pLogicDevice->GetPhysicalDevice()->FindMemoryType(memoryRequirements.memoryTypeBits, info.memoryPropertyFlags),
        };
        CALL_VK(vkAllocateMemory(m_pLogicDevice->GetHandle(), &memoryAllocateInfo, nullptr, &m_pDeviceMemory));

        vkBindImageMemory(m_pLogicDevice->GetHandle(), m_pHandle, m_pDeviceMemory, 0);

        if (info.objName) {
            m_pLogicDevice->SetDebugUtilsObjectName(VK_OBJECT_TYPE_IMAGE, (uint64_t)m_pHandle, info.objName);
        }
    }

    // 创建图像视图
    VkImageViewCreateInfo imageViewCreateInfo {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = m_pHandle,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = info.format,
        .components = {
            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .a = VK_COMPONENT_SWIZZLE_IDENTITY,
        },
        .subresourceRange = {
            .aspectMask = info.aspectFlags,
            .baseMipLevel = 0,
            .levelCount = info.mipLevels,
            .baseArrayLayer = 0,
            .layerCount = 1
        },
    };
    CALL_VK(vkCreateImageView(m_pLogicDevice->GetHandle(), &imageViewCreateInfo, nullptr, &m_pImageView));
}

VulImage2D::~VulImage2D() {
    vkDestroyImage(m_pLogicDevice->GetHandle(), m_pHandle, nullptr);
    vkFreeMemory(m_pLogicDevice->GetHandle(), m_pDeviceMemory, nullptr);
    vkDestroyImageView(m_pLogicDevice->GetHandle(), m_pImageView, nullptr);
	m_pLogicDevice->SubRef();
}

USING_GPU_NAMESPACE_END