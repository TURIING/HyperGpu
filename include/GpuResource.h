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
		struct Image2DCreateInfo: CommonInfo {
			Size size;
			PixelFormat format = PixelFormat::R8G8B8A8;
			ImageAspectFlags aspect = ImageAspectFlags::Color;
			ImageUsageFlags usage = ImageUsageFlags::SAMPLED;
			Sampler* pSampler = nullptr;
		};

		virtual ImageAspectFlags GetAspectFlags() const = 0;
		virtual ImageUsageFlags GetUsage() const = 0;
		virtual Size GetSize() const = 0;
		virtual PixelFormat GetPixelFormat() const = 0;
	};

	class Buffer : public GpuObject {
	public:
		enum BufferType { Vertex, Index, Uniform, TransferSrc, TransferDst, ShaderStorage };
		struct BufferCreateInfo {
			BufferType bufferType = BufferType::Uniform;
			uint64_t bufferSize = 0;
			const void *data = nullptr;				// 当buffer类型为uniform时，该字段为nullptr就行
		};
		virtual void WriteData(const void* data, uint64_t dataSize) = 0;
		virtual void Map(uint64_t offset, uint64_t size, void **pData) = 0;
		virtual void UnMap() = 0;
	};

    enum class AttributeDataType { Float, Int, Vec2, Vec3, Vec4, Mat4 };

    struct VertexAttribute {
        uint8_t location = 0;
        AttributeDataType dataType = AttributeDataType::Float;
    };

    struct InputAssemblerInfo {
        uint32_t attributeCount = 0;
        const VertexAttribute *pAttributes = nullptr;

        const void *pVertexData = nullptr;
        uint32_t vertexSize  = 0;

        const void *pIndexData = nullptr;
        uint32_t indexSize = 0;

        uint32_t vertexCount = 0;
        uint32_t indexCount = 0;
    };

	struct InstanceInputAssemblerInfo final: InputAssemblerInfo {
		uint32_t instanceAttributeCount = 0;
		const VertexAttribute *pInstanceAttributes = nullptr;

		const void *pInstanceData = nullptr;
		uint32_t instanceDataSize = 0;

		uint32_t instanceCount = 0;
	};

    class InputAssembler: public GpuObject {};

	class GpuResourceManager : public GpuObject {
	public:
		virtual Image2D* CreateImage2D(const Image2D::Image2DCreateInfo& info) = 0;
		virtual Buffer*  CreateBuffer(const Buffer::BufferCreateInfo& createInfo) = 0;
		virtual Sampler* CreateSampler(const Sampler::SamplerCreateInfo& info) = 0;
        virtual InputAssembler* CreateInputAssembler(const InputAssemblerInfo &info) = 0;
        virtual InputAssembler* CreateInputAssembler(const InstanceInputAssemblerInfo &info) = 0;
		void DestroyBuffer(Buffer* buffer) { buffer->SubRef(); }
		void DestroyImage(Image2D* image) { image->SubRef(); }
		void DestroySampler(Sampler* sampler) { sampler->SubRef(); }
	};
} // namespace HyperGpu
#endif // GPURESOURCE_H
