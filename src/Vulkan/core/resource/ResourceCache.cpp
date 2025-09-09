/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/4/20 11:26
* @version: 1.0
* @description: 
********************************************************************************/
#include "ResourceCache.h"

#include <typeindex>

#include "../../base/pipeline/VulRenderPass.h"
#include "../../base/surface/VulFrameBuffer.h"
#include "../../core/VulkanDevice.h"
#include "../pipeline/VulkanPipeline.h"
#include "../../base/descriptor/VulDescriptorSet.h"
#include "../../base/pipeline/VulShader.h"

template <>
struct std::hash<Size> {
    std::size_t operator()(const Size &size) const noexcept {
        std::size_t result = 0;
        HASH_COMBINE(result, size.width);
        HASH_COMBINE(result, size.height);
        return result;
    }
};

template <>
struct std::hash<Color> {
    std::size_t operator()(const Color &color) const noexcept {
        std::size_t result = 0;
        HASH_COMBINE(result, color.r);
        HASH_COMBINE(result, color.g);
        HASH_COMBINE(result, color.b);
        HASH_COMBINE(result, color.a);
        return result;
    }
};

template <>
struct std::hash<ShaderInfo> {
    std::size_t operator()(const ShaderInfo &shaderInfo) const noexcept {
        std::size_t result = 0;
        HASH_COMBINE(result, shaderInfo.pSpvVertexCode);
        HASH_COMBINE(result, shaderInfo.spvVertexCodeSize);
        HASH_COMBINE(result, shaderInfo.pSpvFragCode);
        HASH_COMBINE(result, shaderInfo.spvFragCodeSize);
        return result;
    }
};

template <>
struct std::hash<RasterizationInfo> {
    std::size_t operator()(const RasterizationInfo &rasterInfo) const noexcept {
        std::size_t result = 0;
        HASH_COMBINE(result, TO_I32(rasterInfo.primitiveType));
        HASH_COMBINE(result, TO_I32(rasterInfo.polygonMode));
        HASH_COMBINE(result, TO_I32(rasterInfo.cullMode));
        HASH_COMBINE(result, TO_I32(rasterInfo.frontFace));
        HASH_COMBINE(result, rasterInfo.lineWidth);
        return result;
    }
};

template <>
struct std::hash<BlendInfo> {
    std::size_t operator()(const BlendInfo &blendInfo) const noexcept {
        std::size_t result = 0;
        HASH_COMBINE(result, TO_I32(blendInfo.srcColorBlendFactor));
        HASH_COMBINE(result, TO_I32(blendInfo.srcAlphaBlendFactor));
        HASH_COMBINE(result, TO_I32(blendInfo.dstColorBlendFactor));
        HASH_COMBINE(result, TO_I32(blendInfo.dstAlphaBlendFactor));
        HASH_COMBINE(result, TO_I32(blendInfo.colorBlendOp));
        HASH_COMBINE(result, TO_I32(blendInfo.alphaBlendOp));
        HASH_COMBINE(result, blendInfo.constantColor);
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

USING_GPU_NAMESPACE_BEGIN

ResourceCache::ResourceCache(VulkanDevice* pVulkanDevice): m_pVulkanDevice(pVulkanDevice) {
}

ResourceCache::~ResourceCache() {
    for(auto &pair: m_mapFrameBuffers) {
        (*pair.second)->SubRef();
    }

    for(auto &pair: m_mapRenderPasses) {
        (*pair.second)->SubRef();
    }

    for(auto &pair: m_mapPipelines) {
        (*pair.second)->SubRef();
    }
}

VulFrameBuffer* ResourceCache::RequestFrameBuffer(const FrameBufferCacheInfo &info) {
    size_t hash = 0;
    for (auto i = 0; i < info.attachmentCount; i++) {
        hashParam(hash, info.pAttachments[i]);
    }
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
    for (auto i = 0; i < info.attachmentCount; i++) {
        builder.AddAttachmentImageView(info.pAttachments[i]);
    }
    m_mapFrameBuffers[hash] = std::make_unique<VulFrameBuffer*>(builder.Build());

    return *m_mapFrameBuffers[hash];
}

VulRenderPass* ResourceCache::RequestRenderPass(const RenderPassCacheInfo& info) {
    size_t hash = 0;
    hashParam(hash, info.attachmentInfoCount);
    for(auto i = 0; i < info.attachmentInfoCount; i++) {
        hashParam(hash, info.pAttachmentInfos[i].type);
        hashParam(hash, info.pAttachmentInfos[i].attachmentIndex);
        hashParam(hash, info.pAttachmentInfos[i].loadOp);
        hashParam(hash, info.pAttachmentInfos[i].storeOp);
        hashParam(hash, info.pAttachmentInfos[i].format);
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

VulkanPipeline* ResourceCache::RequestPipeline(const RenderEnvInfo &info) {
    size_t hash = 0;
    hashParam(hash, info.shaderInfo);
    hashParam(hash, info.rasterInfo);
    hashParam(hash, info.blendInfo);

    if (const auto it = m_mapPipelines.find(hash); it != m_mapPipelines.end()) {
        return *it->second;
    }

    m_mapPipelines[hash] = std::make_unique<VulkanPipeline*>(new VulkanPipeline(m_pVulkanDevice, info));

    return *m_mapPipelines[hash];
}

VulDescriptorSet* ResourceCache::RequestDescriptorSet(VulkanPipeline *pipeline) {
    if (const auto it = m_mapDescriptorSet.find(pipeline); it != m_mapDescriptorSet.end()) {
        for (auto &setInfo: it->second) {
            if (!setInfo.dirty) {
                setInfo.dirty = true;
                return setInfo.pSet;
            }
        }
    }

    auto pSet = new VulDescriptorSet(
        m_pVulkanDevice->GetLogicDevice(),
        m_pVulkanDevice->GetDescriptorPool(),
        pipeline->GetDescriptorSetLayout(),
        pipeline->GetShader()->GetResourceBinding()
    );
    m_mapDescriptorSet[pipeline].push_back({ pSet, true });
    return pSet;
}

void ResourceCache::ResetAllDescriptorSet(VulkanPipeline *pipeline) {
    LOG_ASSERT(pipeline);
    if (auto it = m_mapDescriptorSet.find(pipeline); it != m_mapDescriptorSet.end()) {
        for (auto &info: it->second) {
            info.dirty = false;
        }
    }
}

void ResourceCache::DeleteAllDescriptorSet(VulkanPipeline *pipeline) {
    LOG_ASSERT(pipeline);
    if (auto it = m_mapDescriptorSet.find(pipeline); it != m_mapDescriptorSet.end()) {
        for (auto &info: it->second) {
            info.pSet->SubRef();
        }
        m_mapDescriptorSet.erase(it);
    }
}

USING_GPU_NAMESPACE_END
