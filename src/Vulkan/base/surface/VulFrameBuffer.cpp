/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/9 21:33
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulFrameBuffer.h"

#include "../device/VulLogicDevice.h"
#include "../pipeline/VulRenderPass.h"

VulFrameBuffer::VulFrameBuffer(VulLogicDevice* device, const VulRenderPass* renderPass, VulFrameBufferCreateInfo &createInfo): m_pLogicDevice(device) {
	m_pLogicDevice->AddRef();

    const VkFramebufferCreateInfo frameBufferCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .renderPass = renderPass->GetHandle(),
        .attachmentCount = static_cast<uint32_t>(createInfo.attachments.size()),
        .pAttachments = createInfo.attachments.data(),
        .width = createInfo.size.width,
        .height = createInfo.size.height,
        .layers = createInfo.layerCount,
    };
    CALL_VK(vkCreateFramebuffer(m_pLogicDevice->GetHandle(), &frameBufferCreateInfo, nullptr, &m_pHandle));
    LOG_INFO("FrameBuffer created");
}

VulFrameBuffer::~VulFrameBuffer() {
    LOG_ASSERT(m_pHandle);
    vkDestroyFramebuffer(m_pLogicDevice->GetHandle(), m_pHandle, nullptr);
	m_pLogicDevice->SubRef();
}

VulFrameBufferBuilder& VulFrameBufferBuilder::AddAttachmentImageView(VkImageView imageView) {
    m_createInfo.attachments.push_back(imageView);
    return *this;
}

VulFrameBuffer* VulFrameBufferBuilder::Build() {
    return new VulFrameBuffer(m_pLogicDevice, m_pRenderPass, m_createInfo);
}
