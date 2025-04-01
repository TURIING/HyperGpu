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
	FrontFace	  frontFace		= FrontFace::COUNTER_CLOCK_WISE;
};

struct AttachmentInfo {
	AttachmentType type;
	uint32_t	   index;
};

struct RenderEnvInfo {
	ShaderInfo					shaderInfo;
	RasterizationInfo			rasterInfo;
	std::vector<AttachmentInfo> attachments;
};

struct InputData {
	std::vector<uint8_t> data;
	uint32_t			 binding;
};

struct InputCollector {
	InputData vertices;
	InputData indices;
};

class Pipeline : public GpuObject {
public:
	virtual ~Pipeline() = default;
};

class PipelineManager : public GpuObject {
public:
	virtual ~						PipelineManager()										 = default;
	[[nodiscard]] virtual Pipeline* CreateRenderPipeline(const RenderEnvInfo& renderEnvInfo) = 0;

	static void DestroyPipeline(Pipeline* pipeline) { pipeline->SubRef(); }
};
}
#endif // GPUPIPELINE_H
