/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/2/24 23:17
* @version: 1.0
* @description: 
********************************************************************************/

#include "VulSwapChain.h"

#include "VulSurface.h"
#include "../device/VulLogicDevice.h"
#include "../device/VulPhysicalDevice.h"
#include "../sync/VulSemaphore.h"
#include "HyperGpu/src/Vulkan/base/resource/VulImage2D.h"

VulSwapChain::VulSwapChain(VulLogicDevice* device, VulSurface* surface): m_pLogicDevice(device), m_pSurface(surface) {
    m_pLogicDevice->AddRef();
    m_pSurface->AddRef();
    const auto physicalDevice = m_pLogicDevice->GetPhysicalDevice();

    this->acquireSwapChainSupportDetails();
    this->chooseSwapSurfaceFormat();
    VkPresentModeKHR presentMode = this->chooseSwapPresentMode();
    VkExtent2D swapExtent = this->getSwapChainExtent();
    uint32_t imageCount = this->getSwapChainImageCount();

    VkSwapchainCreateInfoKHR swapChainCreateInfo {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = surface->GetHandle(),
        .minImageCount = imageCount,
        .imageFormat = m_pSwapChainSurfaceFormat.format,
        .imageColorSpace = m_pSwapChainSurfaceFormat.colorSpace,
        .imageExtent = swapExtent,
        .imageArrayLayers = 1,                                                                  // 指定每个图像所包含的层次。通常，来说它的值为1, 但对于VR相关的应用程序来说，会使用更多的层次
        .imageUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
        // 我们在图像上进行绘制操作，也就是将图像作为一个颜色附着来使用
    };

    // 指定在多个队列族使用交换链图像的方式
    const auto graphicsFamily       = m_pLogicDevice->GetPhysicalDevice()->GetQueueFamily(QueueType::Graphics);
    const auto PresentFamily        = m_pLogicDevice->GetPhysicalDevice()->GetQueueFamily(QueueType::Present);
    uint32_t   queueFamilyIndices[] = {graphicsFamily, PresentFamily};
    if (graphicsFamily != PresentFamily) {
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapChainCreateInfo.queueFamilyIndexCount = 2;
        swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else {
        swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    swapChainCreateInfo.preTransform = m_swapChainSupportDetails.capabilities.currentTransform;
    // 为交换链中的图像指定一个固定的变换操作,比如顺时针旋转90度或是水平翻转
    swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;                             // 用于指定alpha通道是否被用来和窗口系统中的其它窗口进行混合操作。
    swapChainCreateInfo.presentMode = presentMode;
    swapChainCreateInfo.clipped = VK_TRUE;                                                              // 设置为VK TRUE表示我们不关心被窗口系统中的其它窗口遮挡的像素的颜色
    swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;

    CALL_VK(vkCreateSwapchainKHR(m_pLogicDevice->GetHandle(), &swapChainCreateInfo, nullptr, &m_pHandle));
    LOG_INFO("Created SwapChain");

    this->createSwapChainImages();
}

VulSwapChain::~VulSwapChain() {
    vkDestroySwapchainKHR(m_pLogicDevice->GetHandle(), m_pHandle, nullptr);
    m_pSurface->SubRef();
    m_pLogicDevice->SubRef();
}

void VulSwapChain::createSwapChainImages() {
    uint32_t imageCount = 0;
    // images
    vkGetSwapchainImagesKHR(m_pLogicDevice->GetHandle(), m_pHandle, &imageCount, nullptr);
    std::vector<VkImage> images(imageCount);
    vkGetSwapchainImagesKHR(m_pLogicDevice->GetHandle(), m_pHandle, &imageCount, images.data());

    m_vecSwapChainImages.reserve(imageCount);
    for (auto image : images) {
        VulImage2DCreateInfo info{
            .format = m_pSwapChainSurfaceFormat.format,
            .aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT,
            .mipLevels = 1,
            .handle = image,
        };
        m_vecSwapChainImages.emplace_back(new VulImage2D(m_pLogicDevice, info));
    }
}

// 选择合适的呈现模式
VkPresentModeKHR VulSwapChain::chooseSwapPresentMode() const {
    for (const auto& availablePresentMode : m_swapChainSupportDetails.presentModes) {
        if(availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulSwapChain::getSwapChainExtent() {
    const auto extent = m_swapChainSupportDetails.capabilities.currentExtent;
    m_size            = {extent.width, extent.height};
    return extent;
}

uint32_t VulSwapChain::getSwapChainImageCount() const {
    uint32_t imageCount = m_swapChainSupportDetails.capabilities.minImageCount + 1;
    if (m_swapChainSupportDetails.capabilities.maxImageCount > 0 && imageCount > m_swapChainSupportDetails.capabilities.
        maxImageCount) {
        imageCount = m_swapChainSupportDetails.capabilities.maxImageCount;
    }
    return imageCount;
}

VulImage2D* VulSwapChain::GetImage(uint32_t index) const {
    LOG_ASSERT(index >= 0 && index < m_vecSwapChainImages.size());
    return m_vecSwapChainImages[index];
}

VkResult VulSwapChain::AcquireNextImage(VulSemaphore* semaphore, uint32_t &imageIndex) const {
    return vkAcquireNextImageKHR(m_pLogicDevice->GetHandle(), m_pHandle, UINT64_MAX, semaphore->GetHandle(), nullptr,
                                 &imageIndex);
}

void VulSwapChain::acquireSwapChainSupportDetails() {
    auto physicalDevice = m_pLogicDevice->GetPhysicalDevice()->GetHandle();
    auto surface        = m_pSurface->GetHandle();

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &m_swapChainSupportDetails.capabilities);

    uint32_t formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
    if (formatCount != 0) {
        m_swapChainSupportDetails.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount,
                                             m_swapChainSupportDetails.formats.data());
    }

    uint32_t presentModeCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
    if (presentModeCount != 0) {
        m_swapChainSupportDetails.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount,
                                                  m_swapChainSupportDetails.presentModes.data());
    }

    if (m_swapChainSupportDetails.formats.empty() && m_swapChainSupportDetails.presentModes.empty()) {
        LOG_CRITICAL("swap chain formats not supported");
    }
}

// 选择合适的表面格式
void VulSwapChain::chooseSwapSurfaceFormat() {
    for (const auto& availableFormat : m_swapChainSupportDetails.formats) {
        // 1.表示我们以B，G，R和A的顺序,每个颜色通道用8位无符号整型数表示，总共每像素使用32位表示
        // 2.表示SRGB颜色空间是否被支持
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace ==
            VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            m_pSwapChainSurfaceFormat = availableFormat;
            return;
        }
    }
    m_pSwapChainSurfaceFormat = m_swapChainSupportDetails.formats[0];
}
