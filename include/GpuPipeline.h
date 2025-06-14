/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/1 16:00
 * @version: 1.0
 * @description:
 ********************************************************************************/
#ifndef GPUPIPELINE_H
#define GPUPIPELINE_H

#include "Gpu.h"
#include "GpuResource.h"

namespace HyperGpu {
struct ShaderInfo {
	uint8_t* pSpvVertexCode	   = nullptr;
	size_t	 spvVertexCodeSize = 0;
	uint8_t* pSpvFragCode	   = nullptr;
	size_t	 spvFragCodeSize   = 0;
};

struct RasterizationInfo {
	PrimitiveType primitiveType = PrimitiveType::TRIANGLE;
	float		  lineWidth		= 1;
	PolygonMode	  polygonMode	= PolygonMode::FILL;
	CullMode	  cullMode		= CullMode::BACK;
	FrontFace	  frontFace		= FrontFace::CLOCK_WISE;
};

struct AttachmentInfo {
	AttachmentType type;
	uint32_t	   index;
	PixelFormat	   format;
};

struct RenderEnvInfo {
	ShaderInfo					shaderInfo;
	RasterizationInfo			rasterInfo;
	AttachmentInfo*				pAttachment = nullptr;
	uint32_t					attachmentCount = 0;
};

class Pipeline : public GpuObject {};

class PipelineManager : public GpuObject {
public:
	[[nodiscard]] virtual Pipeline* CreateRenderPipeline(const RenderEnvInfo& renderEnvInfo) = 0;

	static void DestroyPipeline(Pipeline* pipeline) { pipeline->SubRef(); }
};
}
#endif // GPUPIPELINE_H
