/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/9 21:06
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULCOMMANDBUFFER_H
#define VULCOMMANDBUFFER_H

#include "../VulObject.h"
#include "../../../common/common.h"

USING_GPU_NAMESPACE_BEGIN

class VulFence;
class VulSemaphore;
class VulPipeline;
class VulFrameBuffer;
class VulRenderPass;
class VulCommandPool;
class VulLogicDevice;
class VulImage2D;

struct VulRenderPassBeginInfo {
    VulRenderPass* pRenderPass = nullptr;
    VulFrameBuffer* pFrameBuffer = nullptr;
    std::vector<VkClearValue> clearValues;
    VkRect2D renderArea;

    [[nodiscard]] VkRenderPassBeginInfo GetRenderPassBeginInfo() const;
};

class VulCommandBuffer final : public VulObject<VkCommandBuffer>{
public:
    VulCommandBuffer(VulLogicDevice* logicDevice, VulCommandPool* commandPool);
    ~VulCommandBuffer() override;
    void Reset() const;
    void BeginRecord(bool isSingleTime = false) const;
    void EndRecord() const;
    void BeginRenderPass(const VulRenderPassBeginInfo& renderPassBeginInfo) const;
    void EndRenderPass() const;
    void BindPipeline(VulPipeline* pipeline) const;
    void SetViewport(const std::vector<VkViewport>& viewports) const;
    void SetScissor(const std::vector<VkRect2D>& scissors) const;
    void BindVertexBuffer(VkBuffer vertexBuffer, VkBuffer instanceBuffer) const;
    void BindIndexBuffer(VkBuffer indexBuffer) const;
	void BindDescriptorSets(VkPipelineBindPoint bindPoint, VkPipelineLayout layout, VkDescriptorSet descriptorSet) const;
    void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) const;
    void PipelineBarrier(VkPipelineStageFlags srcStageFlags, VkPipelineStageFlags dstStageFlags, const VkImageMemoryBarrier& barrier) const;
    void CopyBufferToImage(VkBuffer srcBuffer, VkImage dstImage, VkImageLayout imageLayout, const VkBufferImageCopy &regions) const;
    void CopyImageToBuffer(VkBuffer dstBuffer, VkImage srcImage, VkImageLayout imageLayout, const VkBufferImageCopy &regions) const;
    void ClearColorForImage(VkImage image, const VkClearColorValue& color) const;
    void BlitImage(VulImage2D* pSrcImage, VulImage2D* pDstImage, const std::vector<VkImageBlit> &vecImageBlit, VkFilter filter) const;
    void CopyImage(VulImage2D* pSrcImage, VulImage2D* pDstImage, const std::vector<VkImageCopy> &regions) const;
    void TransitionImageLayout(VulImage2D* pImage, VkImageLayout newLayout, VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT) const;
    void Draw(uint32_t vertexCount, u32 instanceCount = 1) const;
    void DrawIndex(uint32_t indexCount, u32 instanceCount = 1) const;
    void BeginDebugUtilsLabel(const char *name, const Color &color) const;
    void EndDebugUtilsLabel() const;

private:
    void vkBeginDebugUtilsLabelExt(VkCommandBuffer cmd, VkDebugUtilsLabelEXT *label) const;
    void vkEndDebugUtilsLabelExt(VkCommandBuffer cmd) const;

private:
    VulLogicDevice* m_pLogicDevice = nullptr;
    VulCommandPool* m_pCommandPool = nullptr;
};

USING_GPU_NAMESPACE_END
#endif //VULCOMMANDBUFFER_H