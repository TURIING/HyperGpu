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
struct InputAssembler {
	Buffer*  vertexBuffer = nullptr;
	Buffer*  indexBuffer  = nullptr;
	uint32_t vertexCount  = 0;
	uint32_t indexCount   = 0;
};

class GpuCmd : public GpuObject {
public:
	enum class RenderAttachmentType { Image2D, Surface };
	enum class DrawType { Vertex, Index, Instance };

	struct RenderAttachment {
		uint32_t  imageCount = 0;
		Image2D** image		 = nullptr;
	};

	struct DrawInfo {
		InputAssembler* inputAssembler = nullptr;
		DrawType        drawType       = DrawType::Index;
	};

	struct BeginRenderInfo {
		Pipeline*				pipeline = nullptr;
		Viewport				viewport;
		Scissor					scissor;
		std::vector<ClearValue> clearValue;
		Area					renderArea;
		RenderAttachmentType	renderAttachmentType = RenderAttachmentType::Image2D;
		union {
			RenderAttachment renderAttachment;
			GpuSurface*		 surface;
		};
	};

	virtual void Begin(const BeginRenderInfo& beginRenderInfo) = 0;
	virtual void End()										   = 0;
	virtual void Draw(const DrawInfo& info) = 0;
	virtual void ClearColorImage(Image2D* image, Color color) = 0;
};

class GpuCmdManager : public GpuObject {
public:
	[[nodiscard]] virtual GpuCmd* CreateCommandBuffer() = 0;
	virtual void                  WithSingleCmdBuffer(const std::function<void (GpuCmd*)>& func) = 0;
};
}
#endif // GPUCMD_H
