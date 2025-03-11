/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/10 21:06
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULVERTEXBUFFER_H
#define VULVERTEXBUFFER_H

#include "../../../common/common.h"

class VulBuffer;
class VulLogicDevice;

class VulVertexBuffer final {
public:
    VulVertexBuffer(const std::shared_ptr<VulLogicDevice>& device, const std::vector<uint8_t> &bufferVertex);
    [[nodiscard]] VkBuffer GetHandle() const;

private:
    std::shared_ptr<VulLogicDevice> m_pLogicDevice;
    std::unique_ptr<VulBuffer> m_pBuffer;
};



#endif //VULVERTEXBUFFER_H
