/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/2/19 22:39
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULOBJECT_H
#define VULOBJECT_H

#include "Gpu.h"

template <class T>
class VulObject : public GpuObject {
public:
    [[nodiscard]] T GetHandle() const { return m_pHandle; }

protected:
    T m_pHandle = nullptr;
};

#endif //VULOBJECT_H
