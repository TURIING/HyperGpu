/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/5/17 12:08
* @version: 1.0
* @description: 
********************************************************************************/
#include "OpenGlResourceManager.h"

#include "OpenGlDevice.h"
#include "../resource/GlImage2D.h"
#include "../resource/GlBuffer.h"
#include "../resource/GlSampler.h"
#include "../resource/GlInputAssembler.h"

OpenGlResourceManager::OpenGlResourceManager(OpenGlDevice* pDevice): m_pDevice(pDevice) {
    m_pDevice->AddRef();
}

OpenGlResourceManager::~OpenGlResourceManager() {
    m_pDevice->SubRef();
}

Image2D* OpenGlResourceManager::CreateImage2D(const Image2D::Image2DCreateInfo& info) {
    return new GlImage2D(m_pDevice, info);
}

Buffer* OpenGlResourceManager::CreateBuffer(const Buffer::BufferCreateInfo& createInfo) {
    return new GlBuffer(m_pDevice, createInfo);
}

Sampler* OpenGlResourceManager::CreateSampler(const Sampler::SamplerCreateInfo& info) {
    return new GlSampler(m_pDevice, info);
}

InputAssembler* OpenGlResourceManager::CreateInputAssembler(const InputAssemblerInfo& info) {
    return new GlInputAssembler(m_pDevice, info);
}

InputAssembler * OpenGlResourceManager::CreateInputAssembler(const InstanceInputAssemblerInfo &info) {
    return new GlInputAssembler(m_pDevice, info);
}
