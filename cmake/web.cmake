set(VULKAN_LIB ${CMAKE_SOURCE_DIR}/lib/vulkan-1.lib)

add_definitions(-DPLATFORM_WEB)
add_subdirectory(${EXTERNAL}/glew)
set(EXTERNAL_LIB spdlog::spdlog glew)
set(EXTERNAL_INCLUDE ${EXTERNAL})
