/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/8 15:11
 * @version: 1.0
 * @description:
 ********************************************************************************/
#ifndef GPUCMD_H
#define GPUCMD_H

#include "Gpu.h"
#include "GpuPipeline.h"
#include "GpuResource.h"
#include "GpuSurface.h"

namespace HyperGpu {
enum class RenderAttachmentType { Image2D, Surface };

struct RenderAttachment {
	uint32_t  imageCount = 0;
	Image2D** image		 = nullptr;
};

struct UniformBinding {
	Buffer* buffer = nullptr;
	const char* name = nullptr;
};

struct ImageBinding {
	Image2D** pImage = nullptr;
	uint32_t imageCount = 0;
	const char* name = nullptr;
};

struct DrawInfo {
	InputAssembler* pInputAssembler = nullptr;
	UniformBinding* pUniformBinding = nullptr;
	uint32_t uniformBindingCount = 0;
	ImageBinding* pImageBinding = nullptr;
	uint32_t imageBindingCount = 0;
};

struct BeginRenderInfo {
	Pipeline* pPipeline = nullptr;
	std::vector<ClearValue> clearValue;
	Area renderArea;
	RenderAttachmentType renderAttachmentType = RenderAttachmentType::Image2D;
	union {
		RenderAttachment renderAttachment;
		GpuSurface* surface;
	};
};

class GpuCmd : public GpuObject {
public:
	virtual void Reset() = 0;
	virtual void Begin() = 0;
	virtual void End() = 0;
	virtual void BeginRenderPass(const BeginRenderInfo& info) = 0;
	virtual void EndRenderPass() = 0;
	virtual void Draw(const DrawInfo& info) = 0;
	virtual void ClearColorImage(Image2D* image, const Color &color) = 0;
	virtual void SetViewport(const Viewport& viewport) = 0;
	virtual void SetScissor(const Scissor& scissor) = 0;
	virtual void BlitImageToSurface(Image2D* pImage, GpuSurface* surface, ImageBlitRange *pRange, uint32_t rangeCount, Filter filter) = 0;
	virtual void CopyImage(Image2D* pSrcImage, Image2D* pDstImage, ImageCopyRange* pRange, uint32_t rangeCount) = 0;
	virtual void CopyBufferToImage(Image2D* pImage, const void *pData, uint64_t size, const Area& area) = 0;
	virtual void BeginDebugUtilsLabel(const char *name, const Color &color = { 0.0, 0.0, 0.0, 1.0}) = 0;
	virtual void EndDebugUtilsLabel() = 0;
};

class GpuCmdManager : public GpuObject {
public:
	virtual GpuCmd* CreateCommandBuffer() = 0;
	virtual void WithSingleCmdBuffer(const std::function<void (GpuCmd*)>& func) = 0;
};
}
#endif // GPUCMD_H
