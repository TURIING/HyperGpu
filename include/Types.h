/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/2/22 22:27
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef TYPES_H
#define TYPES_H

#include <cstdint>

namespace HyperGpu
{
	enum class PrimitiveType { POINT, LINE, LINE_STRIP, TRIANGLE, TRIANGLE_STRIP };
	enum class PolygonMode { FILL, LINE, POINT };
	enum class CullMode { NONE, FRONT, BACK };
	enum class FrontFace { COUNTER_CLOCK_WISE, CLOCK_WISE };
	enum class AttachmentType { COLOR, DEPTH };
	enum class Filter { NEAREST, LINEAR };
	enum class PixelFormat {
		R8G8B8A8,
		B8G8R8A8,
	};
	enum class WaitState { Success, Failed, Timeout };

	struct Viewport {
		float x      = 0;
		float y      = 0;
		float width  = 0;
		float height = 0;
	};

	struct Scissor {
		int32_t  offsetX = 0;
		int32_t  offsetY = 0;
		uint32_t width   = 0;
		uint32_t height  = 0;
	};

	struct Version {
		int major;
		int minor;
		int patch;
	};

	struct Size {
		uint32_t width  = 0;
		uint32_t height = 0;
	};

	struct Color {
		float r = 0;
		float g = 0;
		float b = 0;
		float a = 0;
	};

	struct DepthStencil {
		float    depth   = 0.0;
		uint32_t stencil = 0;
	};

	struct ClearValue {
		AttachmentType type = AttachmentType::COLOR;
		union {
			Color        color;
			DepthStencil depthStencil;
		};
	};

	struct Offset2D {
		int32_t x = 0;
		int32_t y = 0;
	};

	struct Area {
		Offset2D offset;
		Size     size;
	};

	struct ImageBlitRange {
		Area srcArea;
		Area dstArea;
	};
}
#endif // TYPES_H
