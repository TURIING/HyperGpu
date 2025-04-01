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
class GpuCmd {
public:
	enum class RenderAttachmentType { Image2D, Surface };

	struct RenderAttachment {
		uint32_t  imageCount = 0;
		Image2D** image		 = nullptr;
	};

	struct DrawInfo {
		Buffer*	 vertexBuffer = nullptr;
		Buffer*	 indexBuffer  = nullptr;
		uint32_t vertexCount  = 0;
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

	virtual ~	 GpuCmd()									   = default;
	virtual void Begin(const BeginRenderInfo& beginRenderInfo) = 0;
	virtual void End()										   = 0;
	virtual void Draw(const DrawInfo& info)					   = 0;
};

class GpuCmdManager : public GpuObject {
public:
	virtual ~					  GpuCmdManager()		= default;
	[[nodiscard]] virtual GpuCmd* CreateCommandBuffer() = 0;
};
}
#endif // GPUCMD_H
