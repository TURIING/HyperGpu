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

class VulLogicDevice;
class VulRenderPass;

struct VulAttachmentInfo {
    uint32_t attachmentIndex;
    VkFormat format;
};

struct VulRenderPassCreateInfo {
    bool enableColorAttachment = false;
    bool enableDepthAttachment = false;

    std::vector<VulAttachmentInfo> colorAttachments;
    VulAttachmentInfo depthAttachment;
};

class VulRenderPassBuilder final {
public:
    VulRenderPassBuilder() = default;
    VulRenderPassBuilder& SetLogicDevice(VulLogicDevice* device) { m_pDevice = device; return *this; }
    VulRenderPassBuilder& AddColorAttachment(const VulAttachmentInfo && attachmentInfo);
    VulRenderPassBuilder& AddDepthAttachment(const VulAttachmentInfo && attachmentInfo);
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



#endif //VULRENDERPASS_H
