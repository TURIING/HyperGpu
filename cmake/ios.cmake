set(VULKAN_LIB ${CMAKE_SOURCE_DIR}/lib/vulkan-1.lib)

add_definitions(-DPLATFORM_IOS)
add_definitions(-DVK_USE_PLATFORM_IOS_MVK)
set(EXTERNAL_LIB spdlog::spdlog ${Vulkan_LIBRARIES})
set(EXTERNAL_INCLUDE ${Vulkan_INCLUDE_DIR} ${EXTERNAL}/glm ${EXTERNAL}/spdlog ${EXTERNAL}/SPIRV-Reflect)
