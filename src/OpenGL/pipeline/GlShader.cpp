//
// Created by turiing on 2025/6/28.
//

#include "GlShader.h"
#include "../core/OpenGlDevice.h"
constexpr GLenum gShaderStageToGlShaderStages[] = {
    GL_VERTEX_SHADER,               // Vertex
    GL_FRAGMENT_SHADER,             // Fragment
    GL_GEOMETRY_SHADER,             // Geometry
#if !defined(PLATFORM_MACOS) && !defined(PLATFORM_IOS)
    GL_COMPUTE_SHADER,              // Compute
#endif
};

static void sCheckShaderCompileError(GLuint handle, GlShader::ShaderStage stage) {
    GLint success;
    GLchar infoLog[1024];
    CALL_GL(glGetShaderiv(handle, GL_COMPILE_STATUS, &success));
    if (!success) {
        CALL_GL(glGetShaderInfoLog(handle, 1024, nullptr, infoLog));
        LOG_CRITICAL("SHADER COMPILATION ERROR (STAGE {}): {}", static_cast<int>(stage), infoLog);
    }
}

GlShader::GlShader(OpenGlDevice *pDevice, const char *data, ShaderStage stage): m_pDevice(pDevice) {
    LOG_ASSERT(stage < std::size(gShaderStageToGlShaderStages));
    m_pDevice->AddRef();

    CALL_GL(m_handle = glCreateShader(gShaderStageToGlShaderStages[stage]));

    CALL_GL(glShaderSource(m_handle, 1, &data, nullptr));
    CALL_GL(glCompileShader(m_handle));
    sCheckShaderCompileError(m_handle, stage);
}

GlShader::~GlShader() {
    LOG_ASSERT(m_handle != GL_NONE);

    CALL_GL(glDeleteShader(m_handle));
    m_pDevice->SubRef();
}
