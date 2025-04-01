/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 22:28
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULPIPELINE_H
#define VULPIPELINE_H

#include "../VulObject.h"
#include "../../../common/common.h"
#include "../VulTypeDefine.h"

class VulLogicDevice;

class VulPipeline final : public VulObject<VkPipeline> {
public:
    VulPipeline(VulLogicDevice* device, const VulPipelineState &state);
    ~VulPipeline() override;

private:
    VulLogicDevice* m_pDevice = nullptr;
};

#endif //VULPIPELINE_H