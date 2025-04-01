/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/3/15 12:03
 * @version: 1.0
 * @description:
 ********************************************************************************/
#ifndef GPURESOURCE_H
#define GPURESOURCE_H

#include "Gpu.h"

namespace HyperGpu {
class GpuDevice;
}

namespace HyperGpu {

class Image2D : public GpuObject {
public:
	enum class ImageUsage { Color, Depth, Stencil };
	struct Image2DCreateInfo {
		Size        size;
		PixelFormat format = PixelFormat::R8G8B8A8;
		ImageUsage  usage  = ImageUsage::Color;
	};
	virtual ~Image2D() = default;
};

class Buffer : public GpuObject {
public:
	enum BufferType { Vertex, Index, Uniform };
	virtual ~Buffer() = default;
};

class GpuResourceManager : public GpuObject {
public:
	virtual ~                      GpuResourceManager()                                                      = default;
	[[nodiscard]] virtual Image2D* CreateImage2D(const Image2D::Image2DCreateInfo& info)                     = 0;
	[[nodiscard]] virtual Buffer*  CreateBuffer(Buffer::BufferType type, const uint8_t* data, uint64_t size) = 0;
	static void                    DestroyBuffer(Buffer* buffer) { buffer->SubRef(); }
	static void                    DestroyImage(Image2D* image) { image->SubRef(); }
};
} // namespace HyperGpu
#endif // GPURESOURCE_H
