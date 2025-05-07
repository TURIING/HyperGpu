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
class VulCommandBuffer;

struct VulImage2DCreateInfo {
    Size size;
    VkFormat format;
    VkImageUsageFlags usage;
    VkImageAspectFlags aspectFlags;
    VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;
    VkMemoryPropertyFlags memoryPropertyFlags;
    uint32_t mipLevels = 1;
    VkImage handle = nullptr;
};

class VulImage2D final: public VulObject<VkImage>{
public:
    VulImage2D(VulLogicDevice* device, const VulImage2DCreateInfo& info);
    [[nodiscard]] VkImageView   GetImageViewHandle() const { return m_pImageView; }
    [[nodiscard]] VkImageLayout GetCurrentImageLayout() const { return m_currentLayout; }
    void SetCurrentImageLayout(VkImageLayout layout) { m_currentLayout = layout; }
    [[nodiscard]] uint32_t GetMipLevels() const { return m_mipLevels; }
    ~VulImage2D() override;

private:
    VulLogicDevice* m_pLogicDevice  = nullptr;
    VkImageView     m_pImageView    = nullptr;
    VkDeviceMemory  m_pDeviceMemory = nullptr;
    uint32_t        m_mipLevels     = 0;
    VkImageLayout   m_currentLayout = VK_IMAGE_LAYOUT_UNDEFINED;
};



#endif //VULIMAGE2D_H
