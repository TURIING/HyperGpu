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
    ~GlInputAssembler() override;
    void Bind();
    void UnBind();

private:
    OpenGlDevice *m_pDevice = nullptr;
    GlBuffer *m_pVertexBuffer = nullptr;
    GlBuffer *m_pIndexBuffer = nullptr;

    std::vector<VertexAttribute> m_vertexAttributes;
    uint32_t m_vertexStride = 0;
    GLuint m_primitiveType = GL_NONE;
    uint8_t m_vertexCount = 0;
    uint8_t m_indexCount = 0;

    GLuint m_vao = GL_NONE;
};

USING_GPU_NAMESPACE_END

#endif //GLINPUTASSEMBLER_H
