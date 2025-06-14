/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/8 14:05
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULRENDERPASS_H
#define VULRENDERPASS_H

#include "../../../common/common.h"
#include "../VulObject.h"

USING_GPU_NAMESPACE_BEGIN

class VulLogicDevice;
class VulRenderPass;

struct VulAttachmentInfo {
    AttachmentType type;
    uint32_t attachmentIndex;
    VkFormat format;
};

struct VulRenderPassCreateInfo {
    std::vector<VulAttachmentInfo> attachment;
};

class VulRenderPassBuilder final {
public:
    VulRenderPassBuilder() = default;
    VulRenderPassBuilder& SetLogicDevice(VulLogicDevice* device) { m_pDevice = device; return *this; }
    VulRenderPassBuilder& AddAttachment(const VulAttachmentInfo & attachmentInfo);
    VulRenderPass* Build();

private:
    VulLogicDevice* m_pDevice = nullptr;
    VulRenderPassCreateInfo m_renderPassCreateInfo {};
};

class VulRenderPass final: public VulObject<VkRenderPass> {
public:
    VulRenderPass(VulLogicDevice* device, VulRenderPassCreateInfo &createInfo);
    ~VulRenderPass() override;
    [[nodiscard]] static VulRenderPassBuilder Builder() { return VulRenderPassBuilder{}; }

private:
    VulLogicDevice* m_pDevice = nullptr;
};


USING_GPU_NAMESPACE_END
#endif //VULRENDERPASS_H
