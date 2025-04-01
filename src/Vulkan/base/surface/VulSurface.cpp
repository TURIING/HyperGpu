/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/2/22 23:04
* @version: 1.0
* @description: 
********************************************************************************/

#include "VulSurface.h"
#include "../VulInstance.h"

VulSurface::VulSurface(VulInstance* instance, const void *handle): m_pInstance(instance) {
	m_pInstance->AddRef();

#if PLATFORM_WINDOWS
    const VkWin32SurfaceCreateInfoKHR surfaceCreateInfo{
        .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .hinstance = nullptr,
        .hwnd = (HWND)handle,
    };

    CALL_VK(vkCreateWin32SurfaceKHR(m_pInstance->GetHandle(), &surfaceCreateInfo, nullptr, &m_pSurface));
    LOG_INFO("Successfully created surface!");
#elif PLATFORM_MACOS
    const VkMacOSSurfaceCreateInfoMVK surfaceCreateInfo{
        .sType = VK_STRUCTURE_TYPE_MACOS_SURFACE_CREATE_INFO_MVK,
        .pNext = nullptr,
        .pView = handle,
    };

    CALL_VK(vkCreateMacOSSurfaceMVK(m_pInstance->GetHandle(), &surfaceCreateInfo, nullptr, &m_pHandle));
    LOG_INFO("Successfully created surface!");
#endif
}

VulSurface::~VulSurface() {
    vkDestroySurfaceKHR(m_pInstance->GetHandle(), m_pHandle, nullptr);
	m_pInstance->SubRef();
}
