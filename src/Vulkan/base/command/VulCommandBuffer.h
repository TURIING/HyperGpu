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

class VulFence;
class VulSemaphore;
class VulIndexBuffer;
class VulVertexBuffer;
class VulPipeline;
class VulFrameBuffer;
class VulRenderPass;
class VulCommandPool;
class VulLogicDevice;

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
    void BeginRecord() const;
    void EndRecord() const;
    void BeginRenderPass(const VulRenderPassBeginInfo& renderPassBeginInfo) const;
    void EndRenderPass() const;
    void BindPipeline(VulPipeline* pipeline) const;
    void SetViewport(const std::vector<VkViewport>& viewports) const;
    void SetScissor(const std::vector<VkRect2D>& scissors) const;
    void BindVertexBuffer(VulVertexBuffer* vertexBuffer) const;
    void BindIndexBuffer(VulIndexBuffer* indexBuffer) const;
	void BindDescriptorSets(VkPipelineBindPoint bindPoint, VkPipelineLayout layout, VkDescriptorSet descriptorSet) const;
    void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) const;
    void PipelineBarrier(VkPipelineStageFlags srcStageFlags, VkPipelineStageFlags dstStageFlags, const VkImageMemoryBarrier& barrier) const;
    void FillImageByBuffer(VkBuffer srcBuffer, VkImage dstImage, VkImageLayout imageLayout, const VkBufferImageCopy &regions) const;
	void Draw(uint32_t vertexCount) const;
    void Submit(VulSemaphore* waitSemaphore, VulSemaphore* signalSemaphore, VulFence* inFightFence);
    void Submit();

private:
    VulLogicDevice* m_pLogicDevice = nullptr;
    VulCommandPool* m_pCommandPool = nullptr;
};



#endif //VULCOMMANDBUFFER_H
