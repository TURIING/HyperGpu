/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/9 21:33
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULFRAMEBUFFER_H
#define VULFRAMEBUFFER_H

#include "../VulObject.h"
#include "../../../common/common.h"

class VulFrameBuffer;
class VulRenderPass;
class VulLogicDevice;

struct VulFrameBufferCreateInfo {
    std::vector<VkImageView> attachments;
    Size size;
    uint32_t layerCount = 1;
};

class VulFrameBufferBuilder final {
public:
    VulFrameBufferBuilder& SetLogicDevice(VulLogicDevice* device) { m_pLogicDevice = device; return *this; }
    VulFrameBufferBuilder& SetRenderPass(VulRenderPass* renderPass) { m_pRenderPass = renderPass; return *this; }
    VulFrameBufferBuilder& SetSize(const Size& size) { m_createInfo.size = size; return *this; }
    VulFrameBufferBuilder& SetLayerCount(uint32_t layerCount) { m_createInfo.layerCount = layerCount; return *this; }
    VulFrameBufferBuilder& AddAttachmentImageView(VkImageView imageView);
    [[nodiscard]] VulFrameBuffer* Build();

private:
    VulLogicDevice* m_pLogicDevice = nullptr;
    VulRenderPass* m_pRenderPass = nullptr;
    VulFrameBufferCreateInfo m_createInfo {};
};

class VulFrameBuffer final: public VulObject<VkFramebuffer>{
public:
    VulFrameBuffer(VulLogicDevice* device, const VulRenderPass* renderPass, VulFrameBufferCreateInfo &createInfo);
    ~VulFrameBuffer() override;
    [[nodiscard]] static VulFrameBufferBuilder Builder() { return VulFrameBufferBuilder{}; }

private:
    VulLogicDevice* m_pLogicDevice = nullptr;
};



#endif //VULFRAMEBUFFER_H
