/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/2/22 00:01
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef COREDEFINE_H
#define COREDEFINE_H

#include <memory>
#include <string>

#include "Types.h"
#include "core/GpuPipeline.h"

struct DeviceSupportInfo {

};

class Device {
public:
    virtual ~Device() = default;
    [[nodiscard]] virtual std::shared_ptr<PipelineManager> GetPipelineManager() = 0;
};


struct PlatformWindowInfo {
    void *handle = nullptr;
    Size size;
};

struct DeviceCreateInfo {
    std::string appName;
    Version appVersion;
    PlatformWindowInfo platformWindowInfo;
};

class GpuFactory final {
public:
    enum GpuType { OPENGL, VULKAN };

public:
    virtual ~GpuFactory() = default;
    explicit GpuFactory(GpuType type);
    [[nodiscard]] virtual std::unique_ptr<Device> CreateDevice(const DeviceCreateInfo &info) const;

private:
    GpuType m_type;
};

#endif //COREDEFINE_H

