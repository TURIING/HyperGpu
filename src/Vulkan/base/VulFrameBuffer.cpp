/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/9 21:33
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulFrameBuffer.h"

#include "VulLogicDevice.h"
#include "VulRenderPass.h"

VulFrameBuffer::VulFrameBuffer(const std::shared_ptr<VulLogicDevice>& device, const std::shared_ptr<VulRenderPass>& renderPass, VulFrameBufferCreateInfo &createInfo)
    : m_pLogicDevice(device), m_pRenderPass(renderPass) {

    const VkFramebufferCreateInfo frameBufferCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
        .renderPass = m_pRenderPass->GetHandle(),
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
}
