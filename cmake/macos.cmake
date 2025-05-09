add_definitions(-DVK_USE_PLATFORM_MACOS_MVK)
add_definitions(-DPLATFORM_MACOS)

find_library(OPENGL_LIB OpenGL)
find_library(COCOA_LIB Cocoa)

set(EXTERNAL_LIB spdlog::spdlog ${Vulkan_LIBRARIES} ${OPENGL_LIB} ${COCOA_LIB})
set(EXTERNAL_INCLUDE ${Vulkan_INCLUDE_DIR} ${EXTERNAL}/glm ${EXTERNAL}/spdlog ${EXTERNAL}/SPIRV-Reflect)
