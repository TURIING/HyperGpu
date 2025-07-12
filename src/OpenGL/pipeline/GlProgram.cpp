//
// Created by turiing on 2025/6/29.
//

#include "GlProgram.h"
#include "../core/OpenGlDevice.h"
#include "GlShader.h"

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

void GlProgram::Bind() const {
    CALL_GL(glUseProgram(m_handle));
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
