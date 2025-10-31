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

constexpr GLsizei gAttributeTypeToSize[] = {
    4,          // float
    4,          // int
    8,          // vec2
    12,         // vec3
    16,         // vec4
    64,         // mat4
};

constexpr GLint gAttributeTypeToComponentCount[] = {
    1,          // float
    1,          // int
    2,          // vec2
    3,          // vec3
    4,          // vec4
    16,         // mat4
};

constexpr GLenum gAttributeTypeToComponentType[] = {
    GL_FLOAT,          // float
    GL_INT,            // int
    GL_FLOAT,          // vec2
    GL_FLOAT,          // vec3
    GL_FLOAT,          // vec4
    GL_FLOAT,          // mat4
};

USING_GPU_NAMESPACE_BEGIN
GlInputAssembler::GlInputAssembler(OpenGlDevice* pDevice, const InputAssemblerInfo& info): m_pDevice(pDevice) {
    m_pDevice->AddRef();
    initVertexAndIndexBuffer(info);

    m_pDevice->RunWithContext([&](GlContext*) {
        CALL_GL(glGenVertexArrays(1, &m_vao));
        CALL_GL(glBindVertexArray(m_vao));

        // vertex
        CALL_GL(glBindBuffer(GL_ARRAY_BUFFER, m_pVertexBuffer->GetHandle()));
        int stridePassed = 0;
        for (auto &attr: m_vertexAttributes) {
            stridePassed += gAttributeTypeToSize[static_cast<int>(attr.dataType)];

            CALL_GL(glVertexAttribPointer(
                attr.location,
                gAttributeTypeToComponentCount[static_cast<int>(attr.dataType)],
                gAttributeTypeToComponentType[static_cast<int>(attr.dataType)],
                GL_FALSE,
                m_vertexStride,
                (void*)stridePassed
            ));
            CALL_GL(glEnableVertexAttribArray(attr.location));
        }

        // index
        if (m_pIndexBuffer) {
            CALL_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pIndexBuffer->GetHandle()));
        }
    });
}

GlInputAssembler::GlInputAssembler(OpenGlDevice *pDevice, const InstanceInputAssemblerInfo &info): m_pDevice(pDevice) {
    m_pDevice->AddRef();
    initVertexAndIndexBuffer(info);
    m_instanceCount = info.instanceCount;

    if (info.instanceCount > 0) {
        Buffer::BufferCreateInfo bufferCreateInfo {
            .bufferType = Buffer::Vertex,
            .bufferSize = info.instanceDataSize,
            .data = info.pInstanceData,
        };
        m_pInstanceBuffer = new GlBuffer(m_pDevice, bufferCreateInfo);
    }

    m_instanceVertexAttributes.reserve(info.instanceAttributeCount);
    for (int i = 0; i < info.instanceAttributeCount; i++) {
        m_instanceVertexAttributes.push_back(info.pInstanceAttributes[i]);
        m_instanceVertexStride += gAttributeTypeToSize[static_cast<int>(info.pInstanceAttributes[i].dataType)];
    }

    m_pDevice->RunWithContext([&](GlContext*) {
        CALL_GL(glGenVertexArrays(1, &m_vao));
        CALL_GL(glBindVertexArray(m_vao));

        // vertex
        CALL_GL(glBindBuffer(GL_ARRAY_BUFFER, m_pVertexBuffer->GetHandle()));
        int stridePassed = 0;
        for (auto &attr: m_vertexAttributes) {
            stridePassed += gAttributeTypeToSize[static_cast<int>(attr.dataType)];

            CALL_GL(glVertexAttribPointer(
                attr.location,
                gAttributeTypeToComponentCount[static_cast<int>(attr.dataType)],
                gAttributeTypeToComponentType[static_cast<int>(attr.dataType)],
                GL_FALSE,
                m_vertexStride,
                (void*)stridePassed
            ));
            CALL_GL(glEnableVertexAttribArray(attr.location));
        }

        // instance
        CALL_GL(glBindBuffer(GL_ARRAY_BUFFER, m_pInstanceBuffer->GetHandle()));
        stridePassed = 0;
        for (auto &attr: m_instanceVertexAttributes) {
            stridePassed += gAttributeTypeToSize[static_cast<int>(attr.dataType)];

            CALL_GL(glVertexAttribPointer(
                attr.location,
                gAttributeTypeToComponentCount[static_cast<int>(attr.dataType)],
                gAttributeTypeToComponentType[static_cast<int>(attr.dataType)],
                GL_FALSE,
                m_instanceVertexStride,
                (void*)stridePassed
            ));
            CALL_GL(glVertexAttribDivisor(attr.location, 1));
        }

        // index
        if (m_pIndexBuffer) {
            CALL_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_pIndexBuffer->GetHandle()));
        }
        CALL_GL(glBindVertexArray(0));
        CALL_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
        CALL_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    });
}

GlInputAssembler::~GlInputAssembler() {
    m_pVertexBuffer->SubRef();
    m_pIndexBuffer->SubRef();
    m_pDevice->RunWithContext([&](GlContext*) {
        CALL_GL(glDeleteVertexArrays(1, &m_vao));
    }, false);
    m_pDevice->SubRef();
}

void GlInputAssembler::initVertexAndIndexBuffer(const InputAssemblerInfo &info) {
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
}

void GlInputAssembler::Bind() const {
    CALL_GL(glBindVertexArray(m_vao));
}

void GlInputAssembler::UnBind() {
    CALL_GL(glBindVertexArray(0));
}

USING_GPU_NAMESPACE_END
