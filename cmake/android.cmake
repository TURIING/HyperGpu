find_package(Vulkan REQUIRED)

add_definitions(-DPLATFORM_ANDROID)
add_definitions(-DVK_USE_PLATFORM_ANDROID_KHR)
add_subdirectory(${EXTERNAL}/glew)
set(EXTERNAL_LIB spdlog::spdlog glew ${Vulkan_LIBRARIES})
set(EXTERNAL_INCLUDE ${Vulkan_INCLUDE_DIR} ${EXTERNAL})
file(GLOB_RECURSE SOURCE CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.h)
