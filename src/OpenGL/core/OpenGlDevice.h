/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/2/22 00:34
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef OPENGLDEVICE_H
#define OPENGLDEVICE_H

#include "../../common/common.h"
#include "GpuDevice.h"

using namespace HyperGpu;

class OpenGlDevice final : public GpuDevice {
public:
    OpenGlDevice(const DeviceCreateInfo &info);
    [[nodiscard]] std::shared_ptr<PipelineManager> GetPipelineManager() override;
};
#endif //OPENGLDEVICE_H
