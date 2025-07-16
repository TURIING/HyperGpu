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
    return nullptr;
}

Sampler* OpenGlResourceManager::CreateSampler(const Sampler::SamplerCreateInfo& info) {
    return nullptr;
}

InputAssembler* OpenGlResourceManager::CreateInputAssembler(const InputAssemblerInfo& info) {
    return nullptr;
}

InputAssembler * OpenGlResourceManager::CreateInputAssembler(const InstanceInputAssemblerInfo &info) {
    return nullptr;
}
