/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/8 15:34
 * @version: 1.0
 * @description:
 ********************************************************************************/
#ifndef VULKANCMD_H
#define VULKANCMD_H
#include <GpuCmd.h>
#include <memory>

class VulkanImage2D;
class VulFrameBuffer;
class VulFence;
class VulSemaphore;
class VulCommandBuffer;
class VulCommandPool;
class VulkanDevice;
class VulkanPipeline;

class VulkanCmd final : public GpuCmd {
public:
	VulkanCmd(VulkanDevice* device, VulCommandPool* pool);
	~VulkanCmd() override;
	[[nodiscard]] VulCommandBuffer* GetHandle() const { return m_pCmd; }

	void Reset() override;
	void Begin() override;
	void End() override;
	void BeginRenderPass(const BeginRenderInfo& info) override;
	void EndRenderPass() override;
	void SetViewport(const Viewport& viewport) override;
	void SetScissor(const Scissor& scissor) override;
	void BlitImageToSurface(Image2D* pImage, GpuSurface* surface, const ImageBlitRange& range, Filter filter) override;
	void Draw(const DrawInfo& info) override;
	void ClearColorImage(Image2D* image, Color color) override;

private:
	[[nodiscard]] static VkViewport transViewportToVkViewport(const Viewport& viewport);
	[[nodiscard]] static VkRect2D   transScissorToVkRect2D(const Scissor& scissor);

private:
	VulkanDevice*     m_pVulkanDevice = nullptr;
	VulCommandBuffer* m_pCmd          = nullptr;
    VulkanPipeline* m_pPipeline = nullptr;
	std::unordered_map<size_t, VulFrameBuffer*> m_vecFrameBuffer;
};

#endif // VULKANCMD_H
