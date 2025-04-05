/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/11 22:42
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulImage2D.h"
#include "../VulLogicDevice.h"
#include "../VulPhysicalDevice.h"
#include "../command/VulCommandBuffer.h"

VulImage2D::VulImage2D(VulLogicDevice* device, const VulImage2DCreateInfo& info) : m_pLogicDevice(device), m_mipLevels(info.mipLevels){
	m_pLogicDevice->AddRef();

    const VkImageCreateInfo imageCreateInfo {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType = VK_IMAGE_TYPE_2D,
        .extent = {
            .width = info.size.width,
            .height = info.size.height,
            .depth = 1,
        },
        .mipLevels = info.mipLevels,
        .arrayLayers = 1,
        .format = info.format,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .usage = info.usage,
        .samples = info.samples,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
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

void VulImage2D::TransitionImageLayout(VulCommandBuffer*  pCmd, VkImageLayout newLayout,
                                       VkImageAspectFlags aspectFlags) {
    if (m_currentLayout == newLayout) return;

    VkImageMemoryBarrier barrier {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .oldLayout = m_currentLayout,
        .newLayout = newLayout,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = m_pHandle,
        .subresourceRange = {
            .aspectMask = aspectFlags,
            .baseMipLevel = 0,
            .levelCount = m_mipLevels,
            .baseArrayLayer = 0,
            .layerCount = 1,
        },
    };

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;
    if (m_currentLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage      = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (m_currentLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout ==
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (m_currentLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout ==
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    }
    else if (m_currentLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && newLayout ==
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage      = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else {
        LOG_CRITICAL("VulImage::transitionImageLayout: Unsupported image layout!");
    }

    pCmd->PipelineBarrier(sourceStage, destinationStage, barrier);

    m_currentLayout = newLayout;
}

VulImage2D::~VulImage2D() {
    vkDestroyImage(m_pLogicDevice->GetHandle(), m_pHandle, nullptr);
    vkFreeMemory(m_pLogicDevice->GetHandle(), m_pDeviceMemory, nullptr);
    vkDestroyImageView(m_pLogicDevice->GetHandle(), m_pImageView, nullptr);
	m_pLogicDevice->SubRef();
}
