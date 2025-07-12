/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/4/20 11:26
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef RESOURCECACHE_H
#define RESOURCECACHE_H

#include "../../../common/common.h"

USING_GPU_NAMESPACE_BEGIN

struct VulAttachmentInfo;
class VulkanDevice;
class VulRenderPass;
class VulFrameBuffer;
class VulkanPipeline;

template<typename T>
using ResourceCacheMap = std::unordered_map<std::size_t, std::unique_ptr<T>>;

class ResourceCache final: public GpuObject {
public:
    struct FrameBufferCacheInfo {
        VkImageView* pAttachments = nullptr;
        u32 attachmentCount = 0;
        Size size;
        uint32_t layerCount = 1;
        VulRenderPass* pRenderPass = nullptr;
    };

    struct RenderPassCacheInfo {
        VulAttachmentInfo* pAttachmentInfos = nullptr;
        u32 attachmentInfoCount = 0;
    };

public:
    explicit ResourceCache(VulkanDevice* pVulkanDevice);
    ~ResourceCache() override;
    NODISCARD VulFrameBuffer* RequestFrameBuffer(const FrameBufferCacheInfo &info);
    NODISCARD VulRenderPass* RequestRenderPass(const RenderPassCacheInfo &info);
    NODISCARD VulkanPipeline* RequestPipeline(const RenderEnvInfo &info);

private:
    VulkanDevice* m_pVulkanDevice = nullptr;
    ResourceCacheMap<VulFrameBuffer*> m_mapFrameBuffers;
    ResourceCacheMap<VulRenderPass*> m_mapRenderPasses;
    ResourceCacheMap<VulkanPipeline*> m_mapPipelines;
};

USING_GPU_NAMESPACE_END
#endif //RESOURCECACHE_H
