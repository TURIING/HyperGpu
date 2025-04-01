/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/2/24 23:16
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULSWAPCHAIN_H
#define VULSWAPCHAIN_H

#include "../VulObject.h"
#include "../../../common/common.h"

class VulSemaphore;
class VulSurface;
class VulLogicDevice;

class VulSwapChain final: public VulObject<VkSwapchainKHR>{
public:
    explicit VulSwapChain(VulLogicDevice* device, VulSurface* surface);
    ~VulSwapChain() override;
    [[nodiscard]] VkImage GetImage(uint32_t index) const;
    [[nodiscard]] VkImageView GetImageView(uint32_t index) const;
    [[nodiscard]] uint32_t GetImageCount() const { return m_vecSwapChainImages.size(); }
    [[nodiscard]] VkResult AcquireNextImage(VulSemaphore* semaphore, uint32_t &imageIndex) const;
    [[nodiscard]] Size GetSize() const { return m_size; }

private:
    [[nodiscard]] VkPresentModeKHR chooseSwapPresentMode() const;
    [[nodiscard]] VkExtent2D getSwapChainExtent();
    [[nodiscard]] uint32_t getSwapChainImageCount() const;
    void createSwapChainImagesAndViews();

private:
    VulLogicDevice* m_pLogicDevice = nullptr;
    std::vector<VkImage> m_vecSwapChainImages;
    std::vector<VkImageView> m_vecSwapChainImageViews;
    Size m_size;
};

#endif //VULSWAPCHAIN_H
