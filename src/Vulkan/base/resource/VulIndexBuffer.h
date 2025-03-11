/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/10 21:33
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULINDEXBUFFER_H
#define VULINDEXBUFFER_H

#include "../../../common/common.h"

class VulBuffer;
class VulLogicDevice;

class VulIndexBuffer {
public:
    VulIndexBuffer(const std::shared_ptr<VulLogicDevice>& device, const std::vector<uint8_t> &bufferVertex);
    [[nodiscard]] VkBuffer GetHandle() const;

private:
    std::shared_ptr<VulLogicDevice> m_pLogicDevice;
    std::unique_ptr<VulBuffer> m_pBuffer;};



#endif //VULINDEXBUFFER_H
