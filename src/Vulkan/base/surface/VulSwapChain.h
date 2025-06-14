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

USING_GPU_NAMESPACE_BEGIN

class VulImage2D;
class VulSemaphore;
class VulSurface;
class VulLogicDevice;

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR		capabilities{};
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR>	presentModes;
};

class VulSwapChain final: public VulObject<VkSwapchainKHR>{
public:
    explicit VulSwapChain(VulLogicDevice* device, VulSurface* surface);
    ~VulSwapChain() override;
    [[nodiscard]] VulImage2D* GetImage(uint32_t index) const;
    [[nodiscard]] uint32_t GetImageCount() const { return m_vecSwapChainImages.size(); }
    [[nodiscard]] VkResult AcquireNextImage(VulSemaphore* semaphore, uint32_t &imageIndex) const;
    [[nodiscard]] Size GetSize() const { return m_size; }
    [[nodiscard]] VkFormat GetColorFormat() const { return m_pSwapChainSurfaceFormat.format; }
    [[nodiscard]] VkColorSpaceKHR GetColorSpace() const { return m_pSwapChainSurfaceFormat.colorSpace; }

private:
    [[nodiscard]] VkPresentModeKHR chooseSwapPresentMode() const;
    [[nodiscard]] VkExtent2D getSwapChainExtent();
    [[nodiscard]] uint32_t getSwapChainImageCount() const;
    void createSwapChainImages();
    void acquireSwapChainSupportDetails();
    void chooseSwapSurfaceFormat();

private:
    VulLogicDevice* m_pLogicDevice = nullptr;
    VulSurface* m_pSurface = nullptr;
    SwapChainSupportDetails	m_swapChainSupportDetails;
	VkSurfaceFormatKHR m_pSwapChainSurfaceFormat{};
    std::vector<VulImage2D*> m_vecSwapChainImages;
    Size m_size;
};

USING_GPU_NAMESPACE_END
#endif //VULSWAPCHAIN_H
