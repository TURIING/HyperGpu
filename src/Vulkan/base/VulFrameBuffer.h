/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/9 21:33
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULFRAMEBUFFER_H
#define VULFRAMEBUFFER_H

#include "VulObject.h"
#include "../../common/common.h"

class VulRenderPass;
class VulLogicDevice;

struct VulFrameBufferCreateInfo {
    std::vector<VkImageView> attachments;
    Size size;
    uint32_t layerCount = 1;
};

class VulFrameBuffer final: public VulObject<VkFramebuffer>{
public:
    VulFrameBuffer(const std::shared_ptr<VulLogicDevice>& device, const std::shared_ptr<VulRenderPass>& renderPass, VulFrameBufferCreateInfo &createInfo);
    ~VulFrameBuffer() override;

private:
    std::shared_ptr<VulLogicDevice> m_pLogicDevice;
    std::shared_ptr<VulRenderPass> m_pRenderPass;
};



#endif //VULFRAMEBUFFER_H
