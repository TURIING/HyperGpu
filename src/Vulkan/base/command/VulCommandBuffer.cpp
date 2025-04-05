/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/9 21:06
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulCommandBuffer.h"

#include "VulCommandPool.h"
#include "../surface/VulFrameBuffer.h"
#include "../VulLogicDevice.h"
#include "../pipeline/VulPipeline.h"
#include "../pipeline/VulRenderPass.h"
#include "../resource/VulIndexBuffer.h"
#include "../resource/VulVertexBuffer.h"
#include "../sync/VulFence.h"
#include "../sync/VulSemaphore.h"

VulCommandBuffer::VulCommandBuffer(VulLogicDevice* logicDevice, VulCommandPool* commandPool) : m_pLogicDevice(logicDevice), m_pCommandPool(commandPool) {
	m_pLogicDevice->AddRef();
	m_pCommandPool->AddRef();

    VkCommandBufferAllocateInfo allocInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = m_pCommandPool->GetHandle(),
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,                                                   // 可以被提交到队列进行执行，但不能被其它指令缓冲对象调用
        .commandBufferCount = 1,
    };
    CALL_VK(vkAllocateCommandBuffers(m_pLogicDevice->GetHandle(), &allocInfo, &m_pHandle));
    LOG_INFO("Command buffer created");
}

VulCommandBuffer::~VulCommandBuffer() {
    vkFreeCommandBuffers(m_pLogicDevice->GetHandle(), m_pCommandPool->GetHandle(), 1, &m_pHandle);
	m_pCommandPool->SubRef();
	m_pLogicDevice->SubRef();
}

void VulCommandBuffer::Reset() const {
    vkResetCommandBuffer(m_pHandle, 0);
}

void VulCommandBuffer::BeginRecord(bool isSingleTime) const {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    if (isSingleTime) {
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    }

    CALL_VK(vkBeginCommandBuffer(m_pHandle, &beginInfo));
}

void VulCommandBuffer::EndRecord() const {
    CALL_VK(vkEndCommandBuffer(m_pHandle));
}

void VulCommandBuffer::BeginRenderPass(const VulRenderPassBeginInfo& renderPassBeginInfo) const {
    const auto renderPassInfo = renderPassBeginInfo.GetRenderPassBeginInfo();
    vkCmdBeginRenderPass(m_pHandle, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void VulCommandBuffer::EndRenderPass() const {
    vkCmdEndRenderPass(m_pHandle);
}

void VulCommandBuffer::BindPipeline(VulPipeline* pipeline) const {
    vkCmdBindPipeline(m_pHandle, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetHandle());
}

void VulCommandBuffer::SetViewport(const std::vector<VkViewport>& viewports) const {
    vkCmdSetViewport(m_pHandle, 0, viewports.size(), viewports.data());
}

void VulCommandBuffer::SetScissor(const std::vector<VkRect2D>& scissors) const {
    vkCmdSetScissor(m_pHandle, 0, scissors.size(), scissors.data());
}

void VulCommandBuffer::BindVertexBuffer(VulVertexBuffer* vertexBuffer) const {
    const VkBuffer vertexBuffers[] = { vertexBuffer->GetHandle() };
    constexpr VkDeviceSize offset[] = { 0 };
    vkCmdBindVertexBuffers(m_pHandle, 0, 1, vertexBuffers, offset);
}

void VulCommandBuffer::BindIndexBuffer(VulIndexBuffer* indexBuffer) const {
    vkCmdBindIndexBuffer(m_pHandle, indexBuffer->GetHandle(), 0, VK_INDEX_TYPE_UINT32);
}

void VulCommandBuffer::BindDescriptorSets(VkPipelineBindPoint bindPoint, VkPipelineLayout layout, VkDescriptorSet descriptorSet) const {
    vkCmdBindDescriptorSets(m_pHandle, bindPoint, layout, 0, 1, &descriptorSet, 0, nullptr);
}


void VulCommandBuffer::Submit(VulSemaphore* waitSemaphore, VulSemaphore* signalSemaphore, VulFence* inFightFence) {
    VkSemaphore waitSemaphores[] = { waitSemaphore->GetHandle() };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    VkSemaphore signalSemaphores[] = { signalSemaphore->GetHandle() };

    const VkSubmitInfo submitInfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = waitSemaphores,
        .pWaitDstStageMask = waitStages,
        .commandBufferCount = 1,
        .pCommandBuffers = &m_pHandle,
        .signalSemaphoreCount = 1,
        .pSignalSemaphores = signalSemaphores,
    };
    CALL_VK(vkQueueSubmit(m_pLogicDevice->GetGraphicsQueue(), 1, &submitInfo, inFightFence->GetHandle()));
}

void VulCommandBuffer::Submit() {
    const VkSubmitInfo submitInfo {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .commandBufferCount = 1,
        .pCommandBuffers = &m_pHandle,
    };
    CALL_VK(vkQueueSubmit(m_pLogicDevice->GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE));
    CALL_VK(vkQueueWaitIdle(m_pLogicDevice->GetGraphicsQueue()));
}

void VulCommandBuffer::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, const VkDeviceSize size) const {
    const VkBufferCopy copyRegion {
        .size = size,
    };
    vkCmdCopyBuffer(m_pHandle, srcBuffer, dstBuffer, 1, &copyRegion);
}

void VulCommandBuffer::PipelineBarrier(VkPipelineStageFlags srcStageFlags, VkPipelineStageFlags dstStageFlags, const VkImageMemoryBarrier& barrier) const {
    vkCmdPipelineBarrier(m_pHandle, srcStageFlags, dstStageFlags, 0, 0, nullptr, 0, nullptr, 1, &barrier);
}

void VulCommandBuffer::FillImageByBuffer(VkBuffer srcBuffer, VkImage dstImage, VkImageLayout imageLayout, const VkBufferImageCopy& regions) const {
	vkCmdCopyBufferToImage(m_pHandle, srcBuffer, dstImage, imageLayout, 1, &regions);
}

void VulCommandBuffer::ClearColorForImage(VkImage image, const VkClearColorValue& color) const {
    const VkImageSubresourceRange range{
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .baseMipLevel = 0,
        .levelCount = 1,
        .baseArrayLayer = 0,
        .layerCount = 1,
    };
    vkCmdClearColorImage(m_pHandle, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, &color, 1, &range);
}

void VulCommandBuffer::Draw(uint32_t vertexCount) const {
	vkCmdDraw(m_pHandle, vertexCount, 1, 0, 0);
}

void VulCommandBuffer::DrawIndex(uint32_t indexCount) const {
    vkCmdDrawIndexed(m_pHandle, indexCount, 1, 0, 0, 0);
}

VkRenderPassBeginInfo VulRenderPassBeginInfo::GetRenderPassBeginInfo() const {
    return VkRenderPassBeginInfo {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .framebuffer = pFrameBuffer->GetHandle(),
        .renderPass = pRenderPass->GetHandle(),
        .renderArea = renderArea,
        .clearValueCount = static_cast<uint32_t>(clearValues.size()),
        .pClearValues = clearValues.data()
    };
}
