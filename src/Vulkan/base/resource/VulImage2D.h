/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/11 22:42
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULIMAGE2D_H
#define VULIMAGE2D_H
#include "../../../common/common.h"

#include "../VulObject.h"


class VulLogicDevice;

struct VulImage2DCreateInfo {
    Size size;
    VkFormat format;
    VkImageUsageFlags usage;
    VkImageAspectFlags aspectFlags;
    VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;
    VkMemoryPropertyFlags memoryPropertyFlags;
    uint32_t mipLevels = 1;
};

class VulImage2D final: public VulObject<VkImage>{
public:
    VulImage2D(VulLogicDevice* device, const VulImage2DCreateInfo& info);
    void TransitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout, VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT) const;
    [[nodiscard]] VkImageView GetImageViewHandle() const { return m_pImageView; }
    ~VulImage2D() override;

private:
    VulLogicDevice* m_pLogicDevice = nullptr;
    VkImageView m_pImageView = nullptr;
    VkDeviceMemory m_pDeviceMemory = nullptr;
    uint32_t m_mipLevels = 0;
};



#endif //VULIMAGE2D_H
