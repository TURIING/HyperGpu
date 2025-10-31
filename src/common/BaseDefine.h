/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2024/10/27 21:45
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef BASEDEFINE_H
#define BASEDEFINE_H

#include "Types.h"

using namespace HyperGpu;

/************************************************* Marco ************************************************************/
#define USING_GPU_NAMESPACE_BEGIN namespace HyperGpu {
#define USING_GPU_NAMESPACE_END }
#define BIT(x) (1 << (x))
#define CASE_FROM_TO(FROM, TO) case FROM: return TO;
#define NODISCARD [[nodiscard]]

template <typename T>
uint32_t TO_U32(T value) {
    static_assert(std::is_arithmetic_v<T> || std::is_enum_v<T>, "T must be numeric or enum");
    return static_cast<uint32_t>(value);
}

template <typename T>
int32_t TO_I32(T value) {
    static_assert(std::is_arithmetic_v<T> || std::is_enum_v<T>, "T must be numeric or enum");
    return static_cast<int32_t>(value);
}

inline void HASH_COMBINE(size_t &seed, size_t hash) {
    hash += 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= hash;
}

template <class T>
inline void HASH_COMBINE(size_t &seed, const T &v) {
    std::hash<T> hasher;
    HASH_COMBINE(seed, hasher(v));
}

using u32 = uint32_t;
using i32 = int32_t;
/************************************************* Type *************************************************************/
struct ImageInfo {
    unsigned char *data = nullptr;
    Size size;
};

enum class CmdType {
    None,
    BeginRender,
    EndRender,
    BlitImageToSurface,
    ClearColorImage,
    BeginRenderPass,
    EndRenderPass,
    SetViewport,
    SetScissor,
    Draw,
    BeginDebugUtilsLabel,
    EndDebugUtilsLabel,
};

/************************************************* Variable *********************************************************/
constexpr const char *VK_LAYER_KHRONOS_VALIDATION = "VK_LAYER_KHRONOS_validation";

#if PLATFORM_WEB
constexpr int THREAD_NUM = 1;
#else
constexpr int THREAD_NUM = 5;
#endif

/**************************************************** Type Convert ***************************************************/
constexpr VkFormat gPixelFormatToVkFormat[] = {
    VK_FORMAT_R8G8B8A8_UNORM,                    // R8G8B8A8
    VK_FORMAT_B8G8R8A8_UNORM,                    // B8G8R8A8
};

constexpr u32 gPixelFormatToSizeByte[] = {
    4,              // R8G8B8A8
    4,              // B8G8R8A8
};

constexpr VkImageAspectFlags gImageAspectToVkImageAspectFlag[] = {
    VK_IMAGE_ASPECT_COLOR_BIT,          // ImageUsage::Color
    VK_IMAGE_ASPECT_DEPTH_BIT,          // ImageUsage::Depth
    VK_IMAGE_ASPECT_STENCIL_BIT,        // ImageUsage::Stencil
};

constexpr VkBlendFactor gBlendFactorToVkBlendFactor[] = {
    VK_BLEND_FACTOR_ZERO,
    VK_BLEND_FACTOR_ONE,
    VK_BLEND_FACTOR_SRC_COLOR,
    VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
    VK_BLEND_FACTOR_DST_COLOR,
    VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
    VK_BLEND_FACTOR_SRC_ALPHA,
    VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
    VK_BLEND_FACTOR_DST_ALPHA,
    VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
    VK_BLEND_FACTOR_CONSTANT_COLOR,
    VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR,
    VK_BLEND_FACTOR_CONSTANT_ALPHA,
    VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA,
};

constexpr VkBlendOp gBlendOpToVkBlendOp[] = {
    VK_BLEND_OP_ADD,
    VK_BLEND_OP_SUBTRACT,
    VK_BLEND_OP_REVERSE_SUBTRACT,
    VK_BLEND_OP_MIN,
    VK_BLEND_OP_MAX,
};

constexpr VkAttachmentLoadOp gAttachmentLoadOpToVkAttachmentLoadOp[] = {
    VK_ATTACHMENT_LOAD_OP_LOAD,         // LOAD
    VK_ATTACHMENT_LOAD_OP_CLEAR,        // CLEAR
    VK_ATTACHMENT_LOAD_OP_DONT_CARE,    // DONT_CARE
};

constexpr VkAttachmentStoreOp gAttachmentStoreOpToVkAttachmentStoreOp[] = {
    VK_ATTACHMENT_STORE_OP_STORE,       // STORE
    VK_ATTACHMENT_STORE_OP_DONT_CARE,   // DONT_CARE
};

constexpr GLuint gFilterToGlFilter[] = {
    GL_NEAREST,         // NEAREST
    GL_LINEAR,          // LINEAR
};

constexpr GLenum gAddressModeToGLAddressMode[] = {
    GL_REPEAT,                  // REPEAT
    GL_MIRRORED_REPEAT,         // MIRROR
    GL_CLAMP_TO_EDGE,           // CLAMP_TO_EDGE
    GL_CLAMP_TO_BORDER,         // CLAMP_TO_BORDER
};

constexpr GLuint gPixelFormatToGlFormat[] = {
    GL_RGBA,                    // R8G8B8A8
    GL_BGRA,                    // B8G8R8A8
};

constexpr GLuint gPixelFormatToDataFormat[] = {
    GL_UNSIGNED_BYTE,           // R8G8B8A8
    GL_UNSIGNED_BYTE,           // B8G8R8A8
};

constexpr GLuint gPrimitiveTypeToGlType[] = {
    GL_POINTS,              // POINT
    GL_LINES,               // LINE
    GL_LINE_STRIP,          // LINE_STRIP
    GL_TRIANGLES,           // TRIANGLE
    GL_TRIANGLE_STRIP,      // TRIANGLE_STRIP
};

constexpr GLenum gBlendFactorToGlBlendFactor[] = {
    GL_ZERO,
    GL_ONE,
    GL_SRC_COLOR,
    GL_ONE_MINUS_SRC_COLOR,
    GL_DST_COLOR,
    GL_ONE_MINUS_DST_COLOR,
    GL_SRC_ALPHA,
    GL_ONE_MINUS_SRC_ALPHA,
    GL_DST_ALPHA,
    GL_ONE_MINUS_DST_ALPHA,
    GL_CONSTANT_COLOR,
    GL_ONE_MINUS_CONSTANT_COLOR,
    GL_CONSTANT_ALPHA,
    GL_ONE_MINUS_CONSTANT_ALPHA
};

constexpr GLenum gBlendOpToGlBlendOp[] = {
    GL_FUNC_ADD,
    GL_FUNC_SUBTRACT,
    GL_FUNC_REVERSE_SUBTRACT,
    GL_MIN,
    GL_MAX,
};

constexpr GLuint gBufferTypeToGlUsage[] = {
    GL_STATIC_DRAW,           // Vertex
    GL_STATIC_DRAW,           // Index
    GL_DYNAMIC_DRAW,          // Uniform
};

constexpr GLuint gBufferTypeToGlTarget[] = {
    GL_ARRAY_BUFFER,            // Vertex
    GL_ELEMENT_ARRAY_BUFFER,    // Index
    GL_UNIFORM_BUFFER,          // Uniform
};

#endif //BASEDEFINE_H
