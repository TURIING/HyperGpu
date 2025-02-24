set(VULKAN_LIB ${CMAKE_SOURCE_DIR}/lib/vulkan-1.lib)

add_definitions(-DPLATFORM_WINDOWS)
add_definitions(-DVK_USE_PLATFORM_WIN32_KHR)