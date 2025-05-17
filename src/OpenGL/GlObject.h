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

class GlObject {
public:
    [[nodiscard]] uint32_t GetHandle() const { return m_handle; }

protected:
    uint32_t m_handle = 0;
};
#endif //GLOBJECT_H
