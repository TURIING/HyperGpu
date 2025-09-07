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
	const void* pSpvVertexCode	   = nullptr;
	size_t	spvVertexCodeSize = 0;
	const void* pSpvFragCode	   = nullptr;
	size_t	spvFragCodeSize   = 0;

	const void *pGlVertexCode = nullptr;
	const void *pGlFragCode = nullptr;
};

struct RasterizationInfo {
	PrimitiveType primitiveType = PrimitiveType::TRIANGLE;
	float		  lineWidth		= 1;
	PolygonMode	  polygonMode	= PolygonMode::FILL;
	CullMode	  cullMode		= CullMode::BACK;
	FrontFace	  frontFace		= FrontFace::CLOCK_WISE;
};

struct BlendInfo {
	bool enable = false;
	BlendFactor srcColorBlendFactor{};
	BlendFactor dstColorBlendFactor{};
	BlendFactor srcAlphaBlendFactor{};
	BlendFactor dstAlphaBlendFactor{};
	BlendOp colorBlendOp{};
	BlendOp alphaBlendOp{};
	Color constantColor;
};

struct AttachmentInfo {
	AttachmentType type;
	uint32_t index;
	PixelFormat format;
	AttachmentLoadOp loadOp;
	AttachmentStoreOp storeOp;
};

struct RenderEnvInfo {
	const char* objName = nullptr;
	ShaderInfo shaderInfo;
	RasterizationInfo rasterInfo;
	BlendInfo blendInfo;
	std::vector<AttachmentInfo> attachments;
};

class Pipeline : public GpuObject {
public:
	RenderEnvInfo renderEnvInfo;
};

class PipelineManager : public GpuObject {
public:
	[[nodiscard]] virtual Pipeline* CreateRenderPipeline(const RenderEnvInfo& renderEnvInfo) = 0;
	static void DestroyPipeline(Pipeline* pipeline) { pipeline->SubRef(); }
};
}
#endif // GPUPIPELINE_H
