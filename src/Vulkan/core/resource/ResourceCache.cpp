/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/4/20 11:26
* @version: 1.0
* @description: 
********************************************************************************/
#include "ResourceCache.h"

#include "HyperGpu/src/Vulkan/base/pipeline/VulRenderPass.h"
#include "HyperGpu/src/Vulkan/base/surface/VulFrameBuffer.h"
#include "HyperGpu/src/Vulkan/core/VulkanDevice.h"

template <>
struct std::hash<Size> {
    std::size_t operator()(const Size &size) const {
        std::size_t result = 0;
        HASH_COMBINE(result, size.width);
        HASH_COMBINE(result, size.height);
        return result;
    }
};

template <typename T>
inline void hashParam(size_t &seed, const T &value) {
    HASH_COMBINE(seed, value);
}

template <typename T, typename... Args>
inline void hashParam(size_t &seed, const T &firstArg, const Args &...args) {
    hashParam(seed, firstArg);
    hashParam(seed, args...);
}

template <typename T, u32>
inline void hashParam(size_t &seed, T* const &pointer, u32 count) {
    HASH_COMBINE(seed, count);
    for(auto i = 0; i < count; i++) {
        HASH_COMBINE(seed, pointer[i]);
    }
}

ResourceCache::ResourceCache(VulkanDevice* pVulkanDevice): m_pVulkanDevice(pVulkanDevice) {
}

ResourceCache::~ResourceCache() {

    for(auto &pair: m_mapFrameBuffers) {
        (*pair.second)->SubRef();
    }

    for(auto &pair: m_mapRenderPasses) {
        (*pair.second)->SubRef();
    }
}

VulFrameBuffer* ResourceCache::RequestFrameBuffer(const FrameBufferCacheInfo &info) {
    size_t hash = 0;
    hashParam(hash, info.pAttachments, info.attachmentCount);
    hashParam(hash, info.pRenderPass);
    hashParam(hash, info.size);
    hashParam(hash, info.layerCount);

    if(const auto it =  m_mapFrameBuffers.find(hash); it != m_mapFrameBuffers.end()) {
        return *it->second;
    }

    auto builder = VulFrameBuffer::Builder().SetLogicDevice(m_pVulkanDevice->GetLogicDevice())
                                            .SetSize(info.size)
                                            .SetLayerCount(info.layerCount)
                                            .SetRenderPass(info.pRenderPass);
    m_mapFrameBuffers[hash] = std::make_unique<VulFrameBuffer*>(builder.Build());

    return *m_mapFrameBuffers[hash];
}

VulRenderPass* ResourceCache::RequestRenderPass(const RenderPassCacheInfo& info) {
    size_t hash = 0;
    hashParam(hash, info.attachmentInfoCount);
    for(auto i = 0; i < info.attachmentInfoCount; i++) {
        hashParam(hash, info.pAttachmentInfos[i].type);
        hashParam(hash, info.pAttachmentInfos[i].attachmentIndex);
    }

    if(const auto it =  m_mapRenderPasses.find(hash); it != m_mapRenderPasses.end()) {
        return *it->second;
    }

    auto builder = VulRenderPass::Builder().SetLogicDevice(m_pVulkanDevice->GetLogicDevice());
    for(auto i = 0; i < info.attachmentInfoCount; i++) {
        builder.AddAttachment(info.pAttachmentInfos[i]);
    }
    m_mapRenderPasses[hash] = std::make_unique<VulRenderPass*>(builder.Build());

    return *m_mapRenderPasses[hash];
}
