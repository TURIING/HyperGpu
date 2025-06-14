/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/24 15:15
* @version: 1.0
* @description: 
********************************************************************************/
#include "GlBuffer.h"

#include "../core/OpenGlDevice.h"

USING_GPU_NAMESPACE_BEGIN

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

GlBuffer::GlBuffer(OpenGlDevice* pDevice, const BufferCreateInfo& info): m_pDevice(pDevice) {
    m_pDevice->AddRef();

    m_target = gBufferTypeToGlTarget[info.bufferType];
    m_usage = gBufferTypeToGlUsage[info.bufferType];

    m_pDevice->RunWithContext([&](GlContext *pContext) {
        CALL_GL(glGenBuffers(1, &m_handle));
        CALL_GL(glBindBuffer(GL_COPY_WRITE_BUFFER, m_handle));
        CALL_GL(glBufferData(GL_COPY_WRITE_BUFFER, info.bufferSize, info.data, m_usage));
        CALL_GL(glBindBuffer(GL_COPY_WRITE_BUFFER, 0));
    });
}

GlBuffer::~GlBuffer() {
    m_pDevice->RunWithContext([&](GlContext*) {
        CALL_GL(glDeleteBuffers(1, &m_handle));
    });
    m_pDevice->SubRef();
}

void GlBuffer::UpdateData(const uint8_t* data, uint64_t dataSize) {
}

USING_GPU_NAMESPACE_END
