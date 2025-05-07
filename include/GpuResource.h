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
#include "Types.h"

namespace HyperGpu {
	class GpuCmd;

	class Sampler : public GpuObject {
	public:
		enum AddressMode { REPEAT, MIRROR, CLAMP_TO_EDGE, CLAMP_TO_BORDER };
		struct SamplerCreateInfo {
			Filter      magFilter    = LINEAR;
			Filter      minFilter    = LINEAR;
			AddressMode addressModeU = AddressMode::REPEAT;
			AddressMode addressModeV = AddressMode::REPEAT;
			AddressMode addressModeW = AddressMode::REPEAT;
		};
	};

	class Image2D : public GpuObject {
	public:
		enum class ImageUsage { Color, Depth, Stencil };

		struct Image2DCreateInfo {
			Size        size;
			PixelFormat format   = PixelFormat::R8G8B8A8;
			ImageUsage  usage    = ImageUsage::Color;
			Sampler*    pSampler = nullptr;
		};

		virtual void FillPixels(GpuCmd* pCmd, const uint8_t* data, uint64_t dataSize) = 0;
	};

	class Buffer : public GpuObject {
	public:
		enum BufferType { Vertex, Index, Uniform };
		virtual void UpdateData(const uint8_t* data, uint64_t dataSize) = 0;
	};

	class GpuResourceManager : public GpuObject {
	public:
		struct BufferCreateInfo {
			Buffer::BufferType bufferType = Buffer::Uniform;
			uint64_t           bufferSize = 0;
			uint8_t*           data       = nullptr;				// 当buffer类型为uniform时，该字段为nullptr就行
			uint32_t           binding    = 0;
		};

	public:
		[[nodiscard]] virtual Image2D* CreateImage2D(const Image2D::Image2DCreateInfo& info) = 0;
		[[nodiscard]] virtual Buffer*  CreateBuffer(const BufferCreateInfo& createInfo) = 0;
		[[nodiscard]] virtual Sampler* CreateSampler(const Sampler::SamplerCreateInfo& info) = 0;
		static void                    DestroyBuffer(Buffer* buffer) { buffer->SubRef(); }
		static void                    DestroyImage(Image2D* image) { image->SubRef(); }
		static void                    DestroySampler(Sampler* sampler) { sampler->SubRef(); }
	};
} // namespace HyperGpu
#endif // GPURESOURCE_H
