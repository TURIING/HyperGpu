/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/2/22 23:02
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULSURFACE_H
#define VULSURFACE_H

#include "../VulObject.h"
#include "../.././../common/common.h"

USING_GPU_NAMESPACE_BEGIN
class VulInstance;

class VulSurface final : public VulObject<VkSurfaceKHR>{
public:
    VulSurface(VulInstance* instance, const void *handle);
    ~VulSurface() override;

private:
    VulInstance* m_pInstance = nullptr;
};

USING_GPU_NAMESPACE_END
#endif //VULSURFACE_H
