/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 16:00
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef GPUPIPELINE_H
#define GPUPIPELINE_H
#include <vector>
#include <memory>

namespace HyperGpu
{
    struct ShaderInfo {
        uint8_t *pSpvVertexCode = nullptr;
        size_t spvVertexCodeSize = 0;

        uint8_t *pSpvFragCode = nullptr;
        size_t spvFragCodeSize = 0;
    };

    enum class PrimitiveType { POINT, LINE, LINE_STRIP, TRIANGLE, TRIANGLE_STRIP };
    enum class PolygonMode { FILL, LINE, POINT };
    enum class CullMode { NONE, FRONT, BACK };
    enum class FrontFace { COUNTER_CLOCK_WISE, CLOCK_WISE };
    enum class AttachmentType { COLOR, DEPTH };

    struct RasterizationInfo {
        PrimitiveType primitiveType = PrimitiveType::TRIANGLE;
        float lineWidth = 1;
        PolygonMode polygonMode = PolygonMode::FILL;
        CullMode cullMode = CullMode::BACK;
        FrontFace frontFace = FrontFace::COUNTER_CLOCK_WISE;
    };

    struct AttachmentInfo {
        AttachmentType type;
        uint32_t index;
    };

    struct RenderEnvInfo {
        ShaderInfo shaderInfo;
        RasterizationInfo rasterInfo;
        std::vector<AttachmentInfo> attachments;
    };

    class Pipeline {
    public:
        virtual ~Pipeline() = default;
    };

    class PipelineManager {
    public:
        virtual ~PipelineManager() = default;
        virtual std::shared_ptr<Pipeline> CreateRenderPipeline(const RenderEnvInfo& renderEnvInfo) = 0;
    };
}
#endif //GPUPIPELINE_H
