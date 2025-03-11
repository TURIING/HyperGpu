/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/9 21:06
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULCOMMANDBUFFER_H
#define VULCOMMANDBUFFER_H

#include "VulObject.h"
#include "../../common/common.h"

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
    std::shared_ptr<VulRenderPass> pRenderPass;
    std::shared_ptr<VulFrameBuffer> pFrameBuffer;
    std::vector<VkClearValue> clearValues;
    VkRect2D renderArea;

    [[nodiscard]] VkRenderPassBeginInfo GetRenderPassBeginInfo() const;
};

class VulCommandBuffer final : public VulObject<VkCommandBuffer>{
public:
    VulCommandBuffer(const std::shared_ptr<VulLogicDevice>& logicDevice, const std::shared_ptr<VulCommandPool>& commandPool);
    ~VulCommandBuffer() override;
    void Reset() const;
    void BeginRecord() const;
    void EndRecord() const;
    void BeginRenderPass(const VulRenderPassBeginInfo& renderPassBeginInfo) const;
    void EndRenderPass() const;
    void BindPipeline(const std::shared_ptr<VulPipeline>& pipeline) const;
    void SetViewport(const std::vector<VkViewport>& viewports) const;
    void SetScissor(const std::vector<VkRect2D>& scissors) const;
    void BindVertexBuffer(const std::shared_ptr<VulVertexBuffer>& vertexBuffer) const;
    void BindIndexBuffer(const std::shared_ptr<VulIndexBuffer>& indexBuffer) const;
    void Submit(const std::shared_ptr<VulSemaphore> &waitSemaphore, const std::shared_ptr<VulSemaphore> &signalSemaphore, const std::shared_ptr<VulFence> &inFightFence);
    void Submit();

private:
    std::shared_ptr<VulLogicDevice> m_pLogicDevice;
    std::shared_ptr<VulCommandPool> m_pCommandPool;
};



#endif //VULCOMMANDBUFFER_H
