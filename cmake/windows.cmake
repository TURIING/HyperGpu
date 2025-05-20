set(VULKAN_LIB ${CMAKE_SOURCE_DIR}/lib/vulkan-1.lib)

add_definitions(-DPLATFORM_WINDOWS)
add_definitions(-DVK_USE_PLATFORM_WIN32_KHR)
add_subdirectory(${EXTERNAL}/glew)
add_definitions(-DGLEW_STATIC)
add_definitions(-DGLEW_NO_GLU)

set(EXTERNAL_LIB spdlog::spdlog glew opengl32 ${Vulkan_LIBRARIES})
set(EXTERNAL_INCLUDE ${Vulkan_INCLUDE_DIR} ${EXTERNAL}/glm ${EXTERNAL}/spdlog ${EXTERNAL}/SPIRV-Reflect ${EXTERNAL}/glew)
file(GLOB_RECURSE SOURCE CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.h)
