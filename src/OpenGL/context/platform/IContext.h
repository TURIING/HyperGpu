/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/10 10:38
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef HYPERRENDER_ICONTEXT_H
#define HYPERRENDER_ICONTEXT_H

#include "../../../include/Gpu.h"

namespace HyperGpu
{
    class IContext: public GpuObject {
    public:
        virtual void MakeCurrent() = 0;
        virtual void ClearCurrent() = 0;
    };
}
#endif //HYPERRENDER_ICONTEXT_H
