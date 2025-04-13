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

class VulkanCmd final : public GpuCmd {
public:
	VulkanCmd(VulkanDevice* device, VulCommandPool* pool);
	~VulkanCmd() override;
	void Begin(const BeginRenderInfo& beginRenderInfo) override;
	void End() override;
	void Draw(const DrawInfo& info) override;
	void Submit(VulSemaphore* waitSemaphore, VulSemaphore* signalSemaphore, VulFence* inFlightFence) const;
	void ClearColorImage(Image2D* image, Color color) override;
	[[nodiscard]] VulCommandBuffer* GetHandle() const { return m_pCmd; }

private:
	[[nodiscard]] static VkViewport transViewportToVkViewport(const Viewport& viewport);
	[[nodiscard]] static VkRect2D	transScissorToVkRect2D(const Scissor& scissor);

private:
	VulkanDevice*     m_pVulkanDevice = nullptr;
	VulCommandBuffer* m_pCmd          = nullptr;
	bool              m_isBegin       = false;
	std::unordered_map<VulkanImage2D*, VulFrameBuffer*> m_vecFrameBuffer;
};

#endif // VULKANCMD_H
