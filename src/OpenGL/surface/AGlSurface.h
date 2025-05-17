/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/16 22:19
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef AGLSURFACE_H
#define AGLSURFACE_H

#include "../../common/common.h"
#include "GlSurface.h"

USING_GPU_NAMESPACE_BEGIN
class GlContext;

class AGlSurface final: public GlSurface {
public:
    AGlSurface(GlContext* pContext);
};
USING_GPU_NAMESPACE_END
#endif //AGLSURFACE_H
