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

USING_GPU_NAMESPACE_BEGIN

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
	void BlitImageToSurface(Image2D* pImage, GpuSurface* surface, ImageBlitRange *pRange, uint32_t rangeCount, Filter filter) override;
	void CopyImage(Image2D *pSrcImage, Image2D *pDstImage, ImageCopyRange *pRange, uint32_t rangeCount) override;
	void Draw(const DrawInfo& info) override;
	void ClearColorImage(Image2D* image, const Color &color) override;
	void CopyBufferToImage(Image2D *pImage, const void *pData, uint64_t size, const Area &area) override;
	void BeginDebugUtilsLabel(const char *name, const Color &color) override;
	void EndDebugUtilsLabel() override;

private:
	[[nodiscard]] static VkViewport transViewportToVkViewport(const Viewport& viewport);
	[[nodiscard]] static VkRect2D   transScissorToVkRect2D(const Scissor& scissor);

private:
	VulkanDevice* m_pVulkanDevice = nullptr;
	VulCommandBuffer* m_pCmd = nullptr;
    VulkanPipeline* m_pPipeline = nullptr;
};

USING_GPU_NAMESPACE_END
#endif // VULKANCMD_H
