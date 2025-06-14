/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/24 15:07
* @version: 1.0
* @description: 
********************************************************************************/
#include "GlInputAssembler.h"

#include "../core/OpenGlDevice.h"
#include "GlBuffer.h"

constexpr GLuint gPrimitiveTypeToGlType[] = {
    GL_POINTS,              // POINT
    GL_LINES,               // LINE
    GL_LINE_STRIP,          // LINE_STRIP
    GL_TRIANGLES,           // TRIANGLE
    GL_TRIANGLE_STRIP,      // TRIANGLE_STRIP
};

constexpr uint32_t gAttributeTypeToSize[] = {
    4,          // float
    4,          // int
    8,          // vec2
    12,         // vec3
    16,         // vec4
    64,         // mat4
};

USING_GPU_NAMESPACE_BEGIN
GlInputAssembler::GlInputAssembler(OpenGlDevice* pDevice, const InputAssemblerInfo& info): m_pDevice(pDevice) {
    m_pDevice->AddRef();
    m_primitiveType = gPrimitiveTypeToGlType[static_cast<int>((info.primitiveType))];
    m_vertexCount = info.vertexCount;
    m_indexCount = info.indexCount;

    if (info.vertexSize > 0) {
        Buffer::BufferCreateInfo bufferCreateInfo {
            .bufferType = Buffer::Vertex,
            .bufferSize = info.vertexSize,
            .data = info.pVertexData,
        };
        m_pVertexBuffer = new GlBuffer(m_pDevice, bufferCreateInfo);
    }

    if (info.indexSize > 0) {
        Buffer::BufferCreateInfo bufferCreateInfo {
            .bufferType = Buffer::Index,
            .bufferSize = info.indexSize,
            .data = info.pIndexData,
        };
        m_pIndexBuffer = new GlBuffer(m_pDevice, bufferCreateInfo);
    }

    m_vertexAttributes.reserve(info.attributeCount);
    for (int i = 0; i < info.attributeCount; i++) {
        m_vertexAttributes.push_back(info.pAttributes[i]);
        m_vertexStride += gAttributeTypeToSize[static_cast<int>(info.pAttributes[i].dataType)];
    }

    m_pDevice->RunWithContext([&](GlContext*) {
        CALL_GL(glGenVertexArrays(1, &m_vao));
    });
}

GlInputAssembler::~GlInputAssembler() {
    m_pDevice->RunWithContext([&](GlContext*) {
        CALL_GL(glDeleteVertexArrays(1, &m_vao));
    });
    m_pDevice->SubRef();
}

void GlInputAssembler::Bind() {
    CALL_GL(glBindVertexArray(m_vao));

    // vertex
    CALL_GL(glBindBuffer(GL_ARRAY_BUFFER, m_pVertexBuffer->GetHandle()));
    for (const auto& attr : m_vertexAttributes) {

    }
}

void GlInputAssembler::UnBind() {
}

USING_GPU_NAMESPACE_END
