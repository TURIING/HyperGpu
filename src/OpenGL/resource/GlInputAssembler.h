/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/24 15:07
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef GLINPUTASSEMBLER_H
#define GLINPUTASSEMBLER_H

#include "../../common/common.h"

USING_GPU_NAMESPACE_BEGIN
class OpenGlDevice;
class GlBuffer;

class GlInputAssembler final: public InputAssembler {
public:
    GlInputAssembler(OpenGlDevice* pDevice, const InputAssemblerInfo &info);
    GlInputAssembler(OpenGlDevice* pDevice, const InstanceInputAssemblerInfo &info);
    ~GlInputAssembler() override;
    void Bind() const;
    void UnBind();
    NODISCARD u32 GetVertexCount() const { return m_vertexCount; }
    NODISCARD u32 GetIndexCount() const { return m_indexCount; }
    NODISCARD u32 GetInstanceCount() const { return m_instanceCount; }

private:
    void initVertexAndIndexBuffer(const InputAssemblerInfo &info);

private:
    OpenGlDevice *m_pDevice = nullptr;
    GlBuffer* m_pVertexBuffer = nullptr;
    GlBuffer* m_pIndexBuffer = nullptr;
    GlBuffer* m_pInstanceBuffer = nullptr;

    std::vector<VertexAttribute> m_vertexAttributes;
    std::vector<VertexAttribute> m_instanceVertexAttributes;
    int m_vertexStride = 0;
    int m_instanceVertexStride = 0;
    u32 m_vertexCount = 0;
    u32 m_indexCount = 0;
    u32 m_instanceCount = 0;
    GLuint m_vao = GL_NONE;
};

USING_GPU_NAMESPACE_END

#endif //GLINPUTASSEMBLER_H
