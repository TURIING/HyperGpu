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

VulImage2D::VulImage2D(VulLogicDevice* device, const VulImage2DCreateInfo& info)
    : m_pLogicDevice(device), m_aspect(info.aspectFlags), m_size(info.size)
{
	m_pLogicDevice->AddRef();

    if (info.handle) {
        m_pHandle = info.handle;
    }
    else {
        auto usage = info.usage;
        // 生成mipmap需要GPU支持对该格式做线性过滤
        if (info.mipLevels > 1) {
            VkFormatProperties formatProperties;
            vkGetPhysicalDeviceFormatProperties(m_pLogicDevice->GetPhysicalDevice()->GetHandle(), info.format, &formatProperties);

            if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
                throw std::runtime_error("Texture image format does not support linear blitting!");
            }

            // 如果要生成 mipmap，必须包含 VK_IMAGE_USAGE_TRANSFER_SRC_BIT（因为高层要拷贝给低层）
            if ((usage & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) == 0) {
                usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
            }
        }

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
            .usage = usage,
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
