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

USING_GPU_NAMESPACE_BEGIN

class VulkanImage2D;
class VulFrameBuffer;
class VulFence;
class VulSemaphore;
class VulCommandBuffer;
class VulCommandPool;
class VulkanDevice;
class VulkanPipeline;
class VulDescriptorSet;

class VulkanCmd final : public GpuCmd {
public:
	VulkanCmd(VulkanDevice* device, VulCommandPool* pool);
	~VulkanCmd() override;
	NODISCARD VulCommandBuffer* GetHandle() const { return m_pCmd; }

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
	void Dispatch(const DispatchInfo &info) override;
	void ClearColorImage(Image2D* image, const Color &color) override;
	void CopyBufferToImage(Image2D *pImage, const void *pData, uint64_t size, const Area &area) override;
	void CopyImageToBuffer(Image2D *pImage, Buffer* pBuffer, const Area &area) override;
	void BeginDebugUtilsLabel(const char *name, const Color &color) override;
	void EndDebugUtilsLabel() override;
	void GenerateMipmap(Image2D *pImage) override;

private:
	NODISCARD static VkViewport transViewportToVkViewport(const Viewport& viewport);
	NODISCARD static VkRect2D   transScissorToVkRect2D(const Scissor& scissor);
	void SetUniformBuffers(UniformBinding* infos, uint32_t count) const;
	void SetImages(ImageBinding* infos, uint32_t count) const;

private:
	VulkanDevice* m_pVulkanDevice = nullptr;
	VulCommandBuffer* m_pCmd = nullptr;
	VulDescriptorSet* m_pDescriptorSet = nullptr;
    VulkanPipeline* m_pPipeline = nullptr;
	std::vector<VulkanPipeline*> m_vecHistoryPipeline;
};

USING_GPU_NAMESPACE_END
#endif // VULKANCMD_H
