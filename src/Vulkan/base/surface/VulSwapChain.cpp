/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/2/24 23:17
* @version: 1.0
* @description: 
********************************************************************************/

#include "VulSwapChain.h"

#include "VulSurface.h"
#include "../VulLogicDevice.h"
#include "../VulPhysicalDevice.h"
#include "../sync/VulSemaphore.h"

VulSwapChain::VulSwapChain(VulLogicDevice* device, VulSurface* surface): m_pLogicDevice(device) {
	m_pLogicDevice->AddRef();
    const auto physicalDevice = m_pLogicDevice->GetPhysicalDevice();

    VkPresentModeKHR presentMode = this->chooseSwapPresentMode();
    VkExtent2D swapExtent = this->getSwapChainExtent();
    uint32_t imageCount = this->getSwapChainImageCount();

    VkSwapchainCreateInfoKHR swapChainCreateInfo {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = surface->GetHandle(),
        .minImageCount = imageCount,
        .imageFormat = physicalDevice->GetColorFormat(),
        .imageColorSpace = physicalDevice->GetColorSpace(),
        .imageExtent = swapExtent,
        .imageArrayLayers = 1,                                                                  // 指定每个图像所包含的层次。通常，来说它的值为1, 但对于VR相关的应用程序来说，会使用更多的层次
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT                                       // 我们在图像上进行绘制操作，也就是将图像作为一个颜色附着来使用
    };

    // 指定在多个队列族使用交换链图像的方式
    QueueFamilyIndices indices = physicalDevice->GetQueueFamilyIndices();
    uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };
    if(indices.graphicsFamily != indices.presentFamily) {
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapChainCreateInfo.queueFamilyIndexCount = 2;
        swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else {
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    swapChainCreateInfo.preTransform = m_pLogicDevice->GetPhysicalDevice()->GetSwapChainSupportDetails().capabilities.currentTransform;         // 为交换链中的图像指定一个固定的变换操作,比如顺时针旋转90度或是水平翻转
    swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;                             // 用于指定alpha通道是否被用来和窗口系统中的其它窗口进行混合操作。
    swapChainCreateInfo.presentMode = presentMode;
    swapChainCreateInfo.clipped = VK_TRUE;                                                              // 设置为VK TRUE表示我们不关心被窗口系统中的其它窗口遮挡的像素的颜色
    swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

    CALL_VK(vkCreateSwapchainKHR(m_pLogicDevice->GetHandle(), &swapChainCreateInfo, nullptr, &m_pHandle));
    LOG_INFO("Created SwapChain");

    this->createSwapChainImagesAndViews();
}

VulSwapChain::~VulSwapChain() {
    for(const auto pImageView: m_vecSwapChainImageViews) {
        vkDestroyImageView(m_pLogicDevice->GetHandle(), pImageView, nullptr);
    }
    vkDestroySwapchainKHR(m_pLogicDevice->GetHandle(), m_pHandle, nullptr);
	m_pLogicDevice->SubRef();
}

void VulSwapChain::createSwapChainImagesAndViews() {
    uint32_t imageCount = 0;
    // images
    vkGetSwapchainImagesKHR(m_pLogicDevice->GetHandle(), m_pHandle, &imageCount, nullptr);
    m_vecSwapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(m_pLogicDevice->GetHandle(), m_pHandle, &imageCount, m_vecSwapChainImages.data());

    // views
    m_vecSwapChainImageViews.resize(imageCount);
    for(uint32_t i = 0; i < imageCount; i++) {
        VkImageViewCreateInfo imageViewCreateInfo {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = m_vecSwapChainImages[i],
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = m_pLogicDevice->GetPhysicalDevice()->GetColorFormat(),
            .components = {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY,
            },
            .subresourceRange = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1,
            }
        };
        CALL_VK(vkCreateImageView(m_pLogicDevice->GetHandle(), &imageViewCreateInfo, nullptr, &m_vecSwapChainImageViews[i]));
    }
    LOG_INFO("Created SwapChain images");
}

// 选择合适的呈现模式
VkPresentModeKHR VulSwapChain::chooseSwapPresentMode() const {
    for(const auto &availablePresentMode : m_pLogicDevice->GetPhysicalDevice()->GetSwapChainSupportDetails().presentModes) {
        if(availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulSwapChain::getSwapChainExtent() {
    const auto extent = m_pLogicDevice->GetPhysicalDevice()->GetSwapChainSupportDetails().capabilities.currentExtent;
    m_size = { extent.width, extent.height };
    return extent;
}

uint32_t VulSwapChain::getSwapChainImageCount() const {
    const auto detail = m_pLogicDevice->GetPhysicalDevice()->GetSwapChainSupportDetails();
    uint32_t imageCount = detail.capabilities.minImageCount + 1;
    if(detail.capabilities.maxImageCount > 0 && imageCount > detail.capabilities.maxImageCount) {
        imageCount = detail.capabilities.maxImageCount;
    }
    return imageCount;
}

VkImage VulSwapChain::GetImage(uint32_t index) const {
    LOG_ASSERT(index >= 0 && index < m_vecSwapChainImages.size());
    return m_vecSwapChainImages[index];
}

VkImageView VulSwapChain::GetImageView(uint32_t index) const {
    LOG_ASSERT(index >= 0 && index < m_vecSwapChainImageViews.size());
    return m_vecSwapChainImageViews[index];
}

VkResult VulSwapChain::AcquireNextImage(VulSemaphore* semaphore, uint32_t &imageIndex) const {
    return vkAcquireNextImageKHR(m_pLogicDevice->GetHandle(), m_pHandle, UINT64_MAX, semaphore->GetHandle(), nullptr, &imageIndex);
}