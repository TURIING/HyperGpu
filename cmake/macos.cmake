add_definitions(-DVK_USE_PLATFORM_MACOS_MVK)
add_definitions(-DPLATFORM_MACOS)

find_package(Vulkan REQUIRED)

set(EXTERNAL_LIB spdlog::spdlog ${Vulkan_LIBRARIES} "-framework OpenGL" "-framework Cocoa" "-framework QuartzCore" "-framework Metal")
set(EXTERNAL_INCLUDE ${Vulkan_INCLUDE_DIR} ${EXTERNAL}/glm ${EXTERNAL}/spdlog ${EXTERNAL}/SPIRV-Reflect)
file(GLOB_RECURSE SOURCE CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.h ${PROJECT_SOURCE_DIR}/src/*.mm)
