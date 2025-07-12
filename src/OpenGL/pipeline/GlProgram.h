//
// Created by turiing on 2025/6/29.
//

#ifndef GLPROGRAM_H
#define GLPROGRAM_H

#include "../../common/common.h"
#include "../GlObject.h"

USING_GPU_NAMESPACE_BEGIN
class OpenGlDevice;

class GlProgram final: public GpuObject, public GlObject {
    struct UniformBinding {
        std::string name;
        u32 index = 0;
        u32 binding = 0;
        u32 dataSize = 0;
    };
    struct TextureBinding {
        std::string name;
        u32 index = 0;
        u32 binding = 0;
        u32 location = 0;
        GLenum type;
        bool isArray = false;
    };
public:
    GlProgram(OpenGlDevice* pDevice, const ShaderInfo &shaderInfo);
    ~GlProgram() override;
    void Bind() const;
    // void SetUniformBuffer(const )

private:
    void reflectShader();
    void reflectBlockUniforms();
    void reflectTexture();

private:
    OpenGlDevice* m_pDevice = nullptr;
    std::unordered_map<std::string, UniformBinding> m_uniforms;
    std::unordered_map<std::string, TextureBinding> m_textures;
};

USING_GPU_NAMESPACE_END

#endif //GLPROGRAM_H
