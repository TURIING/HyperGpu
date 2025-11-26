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

#define ENUM_FLAG_OPERATORS(T)                                                                                                                                              \
inline T operator~ (T a) { return static_cast<T>( ~static_cast<std::underlying_type<T>::type>(a) ); }                                                                       \
inline T operator| (T a, T b) { return static_cast<T>( static_cast<std::underlying_type<T>::type>(a) | static_cast<std::underlying_type<T>::type>(b) ); }                   \
inline T operator& (T a, T b) { return static_cast<T>( static_cast<std::underlying_type<T>::type>(a) & static_cast<std::underlying_type<T>::type>(b) ); }                   \
inline T operator^ (T a, T b) { return static_cast<T>( static_cast<std::underlying_type<T>::type>(a) ^ static_cast<std::underlying_type<T>::type>(b) ); }                   \
inline T& operator|= (T& a, T b) { return reinterpret_cast<T&>( reinterpret_cast<std::underlying_type<T>::type&>(a) |= static_cast<std::underlying_type<T>::type>(b) ); }   \
inline T& operator&= (T& a, T b) { return reinterpret_cast<T&>( reinterpret_cast<std::underlying_type<T>::type&>(a) &= static_cast<std::underlying_type<T>::type>(b) ); }   \
inline T& operator^= (T& a, T b) { return reinterpret_cast<T&>( reinterpret_cast<std::underlying_type<T>::type&>(a) ^= static_cast<std::underlying_type<T>::type>(b) ); }

template<typename Enum>
constexpr auto to_underlying(Enum e) noexcept {
	return static_cast<std::underlying_type_t<Enum>>(e);
}

template<typename Enum>
constexpr bool has_flag(Enum value, Enum flag) noexcept {
	return (to_underlying(value) & to_underlying(flag)) != 0;
}

namespace HyperGpu
{
	enum class PrimitiveType { POINT, LINE, LINE_STRIP, TRIANGLE, TRIANGLE_STRIP };
	enum class PolygonMode { FILL, LINE, POINT };
	enum class CullMode { NONE, FRONT, BACK };
	enum class FrontFace { COUNTER_CLOCK_WISE, CLOCK_WISE };
	enum class AttachmentType { COLOR, DEPTH_STENCIL };
	enum class Filter { NEAREST, LINEAR };
	enum class AddressMode { REPEAT, MIRROR, CLAMP_TO_EDGE, CLAMP_TO_BORDER };
	enum class PixelFormat {
		R8G8B8A8_SRGB,
		B8G8R8A8_SRGB,
		R16G16B16A16_SFLOAT,
	};
	enum class ImageAspectFlags { Color, Depth_Stencil };
	enum class ImageUsageFlags {
		TRANS_SRC = 1,
		TRANS_DST = 2,
		SAMPLED = 4,
		STORAGE = 8,
		COLOR_ATTACHMENT = 16,
		DEPTH_STENCIL_ATTACHMENT = 32,
	};
	ENUM_FLAG_OPERATORS(ImageUsageFlags);
	enum class WaitState { Success, Failed, Timeout };
	enum class BlendFactor {
		ZERO,
		ONE,
		SRC_COLOR,
		ONE_MINUS_SRC_COLOR,
		DST_COLOR,
		ONE_MINUS_DST_COLOR,
		SRC_ALPHA,
		ONE_MINUS_SRC_ALPHA,
		DST_ALPHA,
		ONE_MINUS_DST_ALPHA,
		CONSTANT_COLOR,
		ONE_MINUS_CONSTANT_COLOR,
		CONSTANT_ALPHA,
		ONE_MINUS_CONSTANT_ALPHA,
	};
	enum class BlendOp {
		ADD,
		SUBTRACT,
		REVERSE_SUBTRACT,
		MIN,
		MAX,
	};
	enum class AttachmentLoadOp {
		LOAD,			// 保留之前内容
		CLEAR,			// 清除为指定值
		DONT_CARE,		// 不关心
	};
	enum class AttachmentStoreOp {
		STORE,			// 保留供后续使用
		DONT_CARE,		// 不保留
	};

	enum class ColorComponentFlags {
		R = 1 << 0,
		G = 1 << 1,
		B = 1 << 2,
		A = 1 << 3,
	};
	ENUM_FLAG_OPERATORS(ColorComponentFlags);

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

	struct CommonInfo {
		const char* objName = nullptr;
	};

	struct Size {
		uint32_t width  = 0;
		uint32_t height = 0;

		Size operator/(uint32_t v) const {
			return { width / v, height / v };
		}

		Size& operator/=(uint32_t v) {
			width  /= v;
			height /= v;
			return *this;
		}
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

	/**
	 * 以左上角为原点, Y轴向下增长
	 */
	struct ImageBlitRange {
		Area srcArea;
		Area dstArea;
	};

	/**
	 * 以左上角为原点, Y轴向下增长
	 */
	struct ImageCopyRange {
		Area srcArea;
		Area dstArea;
	};
}
#endif // TYPES_H
