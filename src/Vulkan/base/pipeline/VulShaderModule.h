/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 17:04
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULSHADERMODULE_H
#define VULSHADERMODULE_H

#include "../VulObject.h"
#include "../../../common/common.h"

USING_GPU_NAMESPACE_BEGIN

class VulLogicDevice;

class VulShaderModule final: public VulObject<VkShaderModule> {
public:
    VulShaderModule(VulLogicDevice* logicDevice, const uint8_t *code, uint32_t size);
    ~VulShaderModule() override;

private:
    VulLogicDevice* m_pLogicDevice = nullptr;
};

USING_GPU_NAMESPACE_END
#endif //VULSHADERMODULE_H
