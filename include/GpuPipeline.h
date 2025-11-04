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
	const void* pSpvVertexCode = nullptr;
	size_t spvVertexCodeSize = 0;
	const void* pSpvFragCode = nullptr;
	size_t spvFragCodeSize = 0;
	const void* pSpvCompCode = nullptr;
	size_t spvCompCodeSize = 0;

	const void *pGlVertexCode = nullptr;
	const void *pGlFragCode = nullptr;
	const void *pGlCompCode = nullptr;
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
	ColorComponentFlags colorWriteMask = ColorComponentFlags::R | ColorComponentFlags::G | ColorComponentFlags::B | ColorComponentFlags::A;
};

struct AttachmentInfo {
	AttachmentType type;
	uint32_t index;
	PixelFormat format;
	AttachmentLoadOp loadOp;
	AttachmentStoreOp storeOp;
};

struct EnvInfo: CommonInfo {
	ShaderInfo shaderInfo;
};

struct RenderEnvInfo: EnvInfo {
	RasterizationInfo rasterInfo;
	BlendInfo blendInfo;
	std::vector<AttachmentInfo> attachments;
};

struct ComputeEnvInfo: EnvInfo {};

class Pipeline: public GpuObject {
public:
	RenderEnvInfo renderEnvInfo;
	ComputeEnvInfo computeEnvInfo;
};

class PipelineManager : public GpuObject {
public:
	[[nodiscard]] virtual Pipeline* CreateGraphicPipeline(const RenderEnvInfo& renderEnvInfo) = 0;
	[[nodiscard]] virtual Pipeline* CreateComputePipeline(const ComputeEnvInfo& computeEnvInfo) = 0;
	static void DestroyPipeline(Pipeline* pipeline) { pipeline->SubRef(); }
};
}
#endif // GPUPIPELINE_H
