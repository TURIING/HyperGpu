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
#include "../device/VulLogicDevice.h"
#include "../pipeline/VulPipeline.h"
#include "../pipeline/VulRenderPass.h"
#include "../resource/VulIndexBuffer.h"
#include "../resource/VulVertexBuffer.h"
#include "../sync/VulFence.h"
#include "../sync/VulSemaphore.h"
#include "../resource/VulImage2D.h"

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

void VulCommandBuffer::BlitImage(VulImage2D*                     pSrcImage, VulImage2D* pDstImage,
                                 const std::vector<VkImageBlit>& vecImageBlit, VkFilter filter) const {
    vkCmdBlitImage(
        m_pHandle,
        pSrcImage->GetHandle(),
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        pDstImage->GetHandle(),
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        vecImageBlit.size(),
        vecImageBlit.data(),
        filter
    );
}

void VulCommandBuffer::Draw(uint32_t vertexCount) const {
	vkCmdDraw(m_pHandle, vertexCount, 1, 0, 0);
}

void VulCommandBuffer::DrawIndex(uint32_t indexCount) const {
    vkCmdDrawIndexed(m_pHandle, indexCount, 1, 0, 0, 0);
}

void VulCommandBuffer::TransitionImageLayout(VulImage2D* pImage, VkImageLayout newLayout, VkImageAspectFlags aspectFlags) const {
    const auto currentLayout = pImage->GetCurrentImageLayout();

    VkImageMemoryBarrier barrier {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .oldLayout = currentLayout,
        .newLayout = newLayout,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = pImage->GetHandle(),
        .subresourceRange = {
                .aspectMask = aspectFlags,
                .baseMipLevel = 0,
                .levelCount = pImage->GetMipLevels(),
                .baseArrayLayer = 0,
                .layerCount = 1,
        },
    };

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;
    if (currentLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage      = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (currentLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (currentLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    }
    else if (currentLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage      = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (currentLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

        sourceStage      = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (currentLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage      = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (currentLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_NONE;

        sourceStage      = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    }
    else if (currentLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_NONE;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage      = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else {
        LOG_CRITICAL("transitionImageLayout: Unsupported image layout!");
    }

    this->PipelineBarrier(sourceStage, destinationStage, barrier);

    pImage->SetCurrentImageLayout(newLayout);
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
