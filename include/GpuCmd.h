/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/8 15:11
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef GPUCMD_H
#define GPUCMD_H

#include "Types.h"

class GpuCmd {
public:
    virtual ~GpuCmd() = default;

};

class GpuCmdManager {
public:
    virtual ~GpuCmdManager() = default;
    [[nodiscard]] virtual std::shared_ptr<GpuCmd> CreateCommandBuffer() = 0;
};

#endif //GPUCMD_H
