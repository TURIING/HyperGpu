//
// Created by turiing on 2025/6/29.
//

#include "GlProgram.h"
#include "../core/OpenGlDevice.h"
#include "GlShader.h"
#include "../resource/GlImage2D.h"
#include "../resource/GlSampler.h"

constexpr int NAME_BUFFER_SIZE = 256;
const std::unordered_map<GLenum, GLenum> gSamplerTypeToTextureType = {
    { GL_SAMPLER_2D, GL_TEXTURE_2D },
    { GL_SAMPLER_2D_ARRAY, GL_TEXTURE_2D_ARRAY },
    { GL_SAMPLER_3D, GL_TEXTURE_3D },
    { GL_SAMPLER_CUBE, GL_TEXTURE_CUBE_MAP },
};

static void sCheckProgramCompileError(GLuint handle) {
    GLint success;
    GLchar infoLog[1024];

    CALL_GL(glGetProgramiv(handle, GL_LINK_STATUS, &success));
    if (!success) {
        CALL_GL(glGetProgramInfoLog(handle, 1024, nullptr, infoLog));
        LOG_CRITICAL("PROGRAM LINKING ERROR: {}", infoLog);
    }
}

GlProgram::GlProgram(OpenGlDevice *pDevice, const ShaderInfo &shaderInfo): m_pDevice(pDevice) {
    m_pDevice->AddRef();
    CALL_GL(m_handle = glCreateProgram());

    GlShader vertexShader(m_pDevice, static_cast<const char*>(shaderInfo.pGlVertexCode), GlShader::Vertex);
    CALL_GL(glAttachShader(m_handle, vertexShader.GetHandle()));

    GlShader fragShader(m_pDevice, static_cast<const char*>(shaderInfo.pGlFragCode), GlShader::Fragment);
    CALL_GL(glAttachShader(m_handle, fragShader.GetHandle()));

    CALL_GL(glLinkProgram(m_handle));
    sCheckProgramCompileError(m_handle);

    reflectShader();
}

GlProgram::~GlProgram() {
    CALL_GL(glDeleteProgram(m_handle));
    m_pDevice->SubRef();
}

void GlProgram::Bind() {
    CALL_GL(glUseProgram(m_handle));
    activateTexture();
}

void GlProgram::SetTexture(const ImageBinding &binding) {
    LOG_ASSERT(binding.pImage && binding.name && binding.imageCount > 0);

    if (!m_textures.contains(binding.name)) {
        LOG_WARNING("shader reflection information does not contain the texture named {}.", binding.name);
        return;
    }

    if (binding.imageCount == 1) {
        auto &pBinding = m_textures[binding.name];
        LOG_ASSERT_INFO(!pBinding.isArray, "The texture type should not be an array.");

        auto pImage = dynamic_cast<GlImage2D*>(binding.pImage[0]);
        pBinding.pImage = pImage;
        pBinding.pSampler = pImage->GetSampler();
    }
    else if (binding.imageCount > 1) {
        auto &pBinding = m_textures[binding.name];
        LOG_ASSERT_INFO(pBinding.isArray, "The texture type should be an array.");

        for (auto i = 0; i < binding.imageCount; i++) {
            auto pImage = dynamic_cast<GlImage2D*>(binding.pImage[i]);
            pBinding.vecImage.push_back(pImage);
        }
        pBinding.pSampler = pBinding.vecImage[0]->GetSampler();
    }
}

void GlProgram::SetUniformBuffer(const UniformBinding &binding) {
    LOG_ASSERT(binding.name && binding.buffer);


}

void GlProgram::reflectShader() {
    reflectBlockUniforms();
    reflectTexture();
}

void GlProgram::reflectBlockUniforms() {
    GLchar varName[NAME_BUFFER_SIZE];
    GLsizei length;
    i32 dataSize = 0;

    GLint blockCount = 0;
    CALL_GL(glGetProgramiv(m_handle, GL_ACTIVE_UNIFORM_BLOCKS, &blockCount));
    for (auto blockIndex = 0; blockIndex < blockCount; ++blockIndex) {
        CALL_GL(glGetActiveUniformBlockName(m_handle, blockIndex, NAME_BUFFER_SIZE - 1, &length, varName));
        varName[length] = '\0';
        char* offset = strchr(varName, '[');
        if (offset) {
            varName[offset - varName] = '\0';
        }

        CALL_GL(glGetActiveUniformBlockiv(m_handle, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &dataSize));
        auto index = glGetUniformBlockIndex(m_handle, varName);

        GLint binding;
        CALL_GL(glGetActiveUniformBlockiv(m_handle, blockIndex, GL_UNIFORM_BLOCK_BINDING, &binding));

        m_uniforms[varName] = {
            .name = varName,
            .index = index,
            .binding = TO_U32(binding),
            .dataSize = TO_U32(dataSize),
        };
    }
}

void GlProgram::reflectTexture() {
    GLchar varName[NAME_BUFFER_SIZE];
    GLsizei length;
    i32 dataSize = 0;
    GLenum type;
    u32 binding = 0;

    GLint count = 0;
    CALL_GL(glGetProgramiv(m_handle, GL_ACTIVE_UNIFORMS, &count));

    for (u32 index = 0; index < count; index++) {
        CALL_GL(glGetActiveUniform(m_handle, index, NAME_BUFFER_SIZE - 1, &length, &dataSize, &type, varName));
        switch (type) {
            case GL_SAMPLER_2D:
            case GL_SAMPLER_2D_ARRAY:
            case GL_SAMPLER_3D:
            case GL_SAMPLER_CUBE: {
                auto it = gSamplerTypeToTextureType.find(type);
                LOG_ASSERT(it != gSamplerTypeToTextureType.end());

                char* offset = strchr(varName, '[');
                bool isArray = false;
                if (offset) {
                    varName[offset - varName] = '\0';
                    isArray = true;
                }

                i32 location;
                CALL_GL(location = glGetUniformLocation(m_handle, varName));
                LOG_ASSERT(location > -1);

                m_textures[varName] = {
                    .name = varName,
                    .index = index,
                    .binding = binding++,
                    .location = TO_U32(location),
                    .type = it->second,
                    .isArray = isArray,
                };
                break;
            }
            default: LOG_ASSERT_INFO(false, "Unknown uniform type.");
        }
    }
}

void GlProgram::activateTexture() {
    int32_t unit = 0;
    for (auto [name, binding] : m_textures) {
        if (binding.isArray) {
            std::vector<GLint> vecUnit;
            for (auto i = 0; i < binding.vecImage.size(); i++) {
                CALL_GL(glActiveTexture(GL_TEXTURE0 + unit + i));
                CALL_GL(glBindTexture(GL_TEXTURE_2D, binding.vecImage[i]->GetHandle()));
                CALL_GL(glBindSampler(unit + i, binding.pSampler->GetHandle()));
                vecUnit.push_back(unit + i);
            }
            CALL_GL(glUniform1iv(TO_I32(binding.location), TO_I32(vecUnit.size()), vecUnit.data()));
        }
        else {
            CALL_GL(glActiveTexture(GL_TEXTURE0 + unit));
            CALL_GL(glBindTexture(GL_TEXTURE_2D, binding.pImage->GetHandle()));
            CALL_GL(glBindSampler(unit, binding.pSampler->GetHandle()));
            CALL_GL(glUniform1i(TO_I32(binding.location), unit));
        }
    }
}
