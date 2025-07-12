/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/17 13:54
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef GLOBJECT_H
#define GLOBJECT_H

#include "../common/common.h"

USING_GPU_NAMESPACE_BEGIN

class GlObject {
public:
    [[nodiscard]] GLuint GetHandle() const { return m_handle; }

protected:
    GLuint m_handle = GL_NONE;
};

USING_GPU_NAMESPACE_END
#endif //GLOBJECT_H
