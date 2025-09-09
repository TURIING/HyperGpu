/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/2/22 00:01
 * @version: 1.0
 * @description:
 ********************************************************************************/
#ifndef COREDEFINE_H
#define COREDEFINE_H

#include <GpuResource.h>

#include "GpuCmd.h"
#include "GpuPipeline.h"
#include "GpuSurface.h"
#include "GpuSync.h"
#include "Types.h"

namespace HyperGpu
{
    enum class QueueType { Graphics, Transfer, Compute, Present };

    struct QueueInfo {
        QueueType type = QueueType::Graphics;
        float priority = 1.0f; // 队列优先级，[0.0 - 1.0]
    };

    class Queue: public GpuObject {
    public:
        enum class WaitStage {
            COLOR_ATTACHMENT_OUTPUT_BIT = 0x00000400, // 颜色附件输出阶段
            COMPUTE_SHADER_BIT          = 0x00000800, // 计算着色器阶段
        };

        struct SubmitInfo {
            Semaphore* const * pWaitSemaphores = nullptr;
            uint32_t waitSemaphoreCount = 0;
            Semaphore* const * pSignalSemaphores = nullptr;
            uint32_t signalSemaphoreCount = 0;
            Fence* pFence = nullptr;
            GpuCmd* const * pCmd = nullptr;
            uint32_t cmdCount = 0;
            WaitStage waitDstStageMask = WaitStage::COLOR_ATTACHMENT_OUTPUT_BIT;
        };

        struct PresentInfo {
            Semaphore** pWaitSemaphores      = nullptr;
            uint32_t    waitSemaphoreCount   = 0;
            GpuSurface* pSurface             = nullptr;
            uint32_t*   imageIndex           = nullptr;
        };

        virtual void Submit(const SubmitInfo& submitInfo) = 0;
        virtual void Present(const PresentInfo& presentInfo) = 0;
        virtual void Wait() = 0;
    };

    struct PlatformWindowInfo {
        void* handle = nullptr;
        Size  size;
    };

    class GpuDevice : public GpuObject {
    public:
        ~GpuDevice() override = default;
        [[nodiscard]] virtual PipelineManager* GetPipelineManager() = 0;
        [[nodiscard]] virtual GpuCmdManager* GetCmdManager() = 0;
        [[nodiscard]] virtual GpuResourceManager* GetResourceManager() = 0;
        [[nodiscard]] virtual GpuSyncManager* GetSyncManager() = 0;
        [[nodiscard]] virtual GpuSurface* CreateSurface(const PlatformWindowInfo& platformWindowInfo) = 0;
        [[nodiscard]] virtual Queue* CreateQueue(QueueType queueType) = 0;
    };

    struct DeviceCreateInfo {
        QueueInfo* pQueueInfo     = nullptr;
        uint32_t   queueInfoCount = 0;
    };

    class GpuFactory final : public GpuObject {
    public:
        enum GpuType { OPENGL, VULKAN };

    public:
        explicit GpuFactory(GpuType type);
        [[nodiscard]] GpuDevice* CreateDevice(const DeviceCreateInfo& info) const;
        static void DestroyDevice(GpuDevice* device) { device->SubRef(); }

    private:
        GpuType m_type;
    };
} // namespace HyperGpu

#endif // COREDEFINE_H
