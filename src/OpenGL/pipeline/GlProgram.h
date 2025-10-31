//
// Created by turiing on 2025/6/29.
//

#ifndef GLPROGRAM_H
#define GLPROGRAM_H

#include "../../common/common.h"
#include "../GlObject.h"

USING_GPU_NAMESPACE_BEGIN
class OpenGlDevice;
class GlImage2D;
class GlSampler;
class GlBuffer;

class GlProgram final: public GpuObject, public GlObject {
    struct UniformBlockBinding {
        std::string name;
        u32 index = 0;
        u32 binding = 0;
        u32 dataSize = 0;
        GlBuffer* pBuffer = nullptr;
    };
    struct TextureBinding {
        std::string name;
        u32 index = 0;
        u32 binding = 0;
        u32 location = 0;
        GLenum type;
        bool isArray = false;           // 标记是否是数组类型，如sampler2D tex[10]

        GlImage2D* pImage = nullptr;
        GlSampler* pSampler = nullptr;
        std::vector<GlImage2D*> vecImage;
    };
public:
    GlProgram(OpenGlDevice* pDevice, const ShaderInfo &shaderInfo);
    ~GlProgram() override;
    void Bind();
    void SetTexture(const ImageBinding &binding);
    void SetUniformBuffer(const UniformBinding &binding);

private:
    void reflectShader();
    void reflectBlockUniforms();
    void reflectTexture();
    void activateTexture();
    void activateUniformBlock();

private:
    OpenGlDevice* m_pDevice = nullptr;
    std::unordered_map<std::string, UniformBlockBinding> m_uniforms;
    std::unordered_map<std::string, TextureBinding> m_textures;
};

USING_GPU_NAMESPACE_END

#endif //GLPROGRAM_H
