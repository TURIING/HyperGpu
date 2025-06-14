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
			Filter      magFilter    = Filter::LINEAR;
			Filter      minFilter    = Filter::LINEAR;
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
		struct BufferCreateInfo {
			BufferType bufferType = BufferType::Uniform;
			uint64_t bufferSize = 0;
			const uint8_t *data = nullptr;				// 当buffer类型为uniform时，该字段为nullptr就行
		};
		virtual void UpdateData(const uint8_t* data, uint64_t dataSize) = 0;
	};

    enum class AttributeDataType { Float, Int, Vec2, Vec3, Vec4, Mat4 };

    struct VertexAttribute {
        uint8_t location = 0;
        AttributeDataType dataType = AttributeDataType::Float;
    };

    struct InputAssemblerInfo {
        uint8_t attributeCount = 0;
        const VertexAttribute *pAttributes = nullptr;

        const uint8_t *pVertexData = nullptr;
        uint32_t vertexSize  = 0;

        const uint8_t *pIndexData = nullptr;
        uint32_t indexSize = 0;

        PrimitiveType primitiveType = PrimitiveType::TRIANGLE;
        uint32_t vertexCount = 0;
        uint32_t indexCount = 0;
    };

    class InputAssembler: public GpuObject {};

	class GpuResourceManager : public GpuObject {
	public:
		virtual Image2D* CreateImage2D(const Image2D::Image2DCreateInfo& info) = 0;
		virtual Buffer*  CreateBuffer(const Buffer::BufferCreateInfo& createInfo) = 0;
		virtual Sampler* CreateSampler(const Sampler::SamplerCreateInfo& info) = 0;
        virtual InputAssembler* CreateInputAssembler(const InputAssemblerInfo &info) = 0;
		void DestroyBuffer(Buffer* buffer) { buffer->SubRef(); }
		void DestroyImage(Image2D* image) { image->SubRef(); }
		void DestroySampler(Sampler* sampler) { sampler->SubRef(); }
	};
} // namespace HyperGpu
#endif // GPURESOURCE_H
