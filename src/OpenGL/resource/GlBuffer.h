/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/24 15:15
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef GLBUFFER_H
#define GLBUFFER_H

#include "../../common/common.h"
#include "../GlObject.h"

USING_GPU_NAMESPACE_BEGIN
class OpenGlDevice;

class GlBuffer final: public Buffer, public GlObject {
public:
    GlBuffer(OpenGlDevice* pDevice, const BufferCreateInfo &info);
    ~GlBuffer() override;
    void UpdateData(const uint8_t* data, uint64_t dataSize) override;

private:
    OpenGlDevice* m_pDevice = nullptr;
    GLuint m_target = GL_NONE;
    GLuint m_usage = GL_NONE;
};

USING_GPU_NAMESPACE_END

#endif //GLBUFFER_H
