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

USING_GPU_NAMESPACE_BEGIN

class VulLogicDevice;

struct VulImage2DCreateInfo {
    Size size;
    VkFormat format;
    VkImageUsageFlags usage;
    VkImageAspectFlags aspectFlags;
    VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;
    VkMemoryPropertyFlags memoryPropertyFlags;
    uint32_t mipLevels = 1;
    VkImage handle = nullptr;
    const char* objName = nullptr;
};

class VulImage2D final: public VulObject<VkImage>{
public:
    VulImage2D(VulLogicDevice* device, const VulImage2DCreateInfo& info);
    ~VulImage2D() override;
    NODISCARD VkImageView GetImageViewHandle() const { return m_pImageView; }
    NODISCARD VkImageLayout GetCurrentImageLayout() const { return m_currentLayout; }
    void SetCurrentImageLayout(VkImageLayout layout) { m_currentLayout = layout; }
    NODISCARD VkImageAspectFlags GetAspectFlag() const { return m_aspect; }
    NODISCARD Size GetSize() const { return m_size; }

private:
    VulLogicDevice* m_pLogicDevice  = nullptr;
    VkImageView     m_pImageView    = nullptr;
    VkDeviceMemory  m_pDeviceMemory = nullptr;
    VkImageLayout   m_currentLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    VkImageAspectFlags m_aspect = VK_IMAGE_ASPECT_COLOR_BIT;
    Size m_size;
};

USING_GPU_NAMESPACE_END
#endif //VULIMAGE2D_H
