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

USING_GPU_NAMESPACE_BEGIN
class GlContext;
class GlQueue;
class GlThreadPool;

class OpenGlDevice final : public GpuDevice {
public:
    explicit OpenGlDevice(const DeviceCreateInfo &info);
    ~OpenGlDevice() override;
	[[nodiscard]] PipelineManager*    GetPipelineManager() override;
	[[nodiscard]] GpuCmdManager*      GetCmdManager() override;
	[[nodiscard]] GpuResourceManager* GetResourceManager() override;
	[[nodiscard]] GpuSyncManager*     GetSyncManager() override;
    GpuSurface *                      CreateSurface(const PlatformWindowInfo &platformWindowInfo) override;
    Queue *                           CreateQueue(QueueType queueType) override;
    GlContext*                        CreateContext();
	void                              RunWithContext(std::function<void(GlContext*)> func, bool waitFinish) const;
	GlThreadPool*                     GetThreadPool() const { return m_pThreadPool; }

private:
    GlContext* m_pMainContext = nullptr;
	std::vector<GlQueue*> m_vecQueue;
	GlQueue* m_pOneTimeQueue = nullptr;
	GlThreadPool* m_pThreadPool = nullptr;
};

USING_GPU_NAMESPACE_END

#endif //OPENGLDEVICE_H
