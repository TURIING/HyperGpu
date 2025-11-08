find_package(Vulkan REQUIRED)

add_definitions(-DPLATFORM_WINDOWS)
add_definitions(-DVK_USE_PLATFORM_WIN32_KHR)
add_subdirectory(${EXTERNAL}/glew)
add_definitions(-DGLEW_STATIC)
add_definitions(-DGLEW_NO_GLU)
#add_definitions(-DWGL_CONTEXT)

set(EXTERNAL_LIB spdlog::spdlog glew opengl32 ${Vulkan_LIBRARIES} libEGL libGLES_CM libGLESv2)
set(EXTERNAL_INCLUDE
        ${Vulkan_INCLUDE_DIR}
        ${EXTERNAL}/glm
        ${EXTERNAL}/spdlog
        ${EXTERNAL}/SPIRV-Reflect
        ${EXTERNAL}/glew
        ${EXTERNAL}/OpenGL/api
        ${EXTERNAL}/EGL/api
)
file(GLOB_RECURSE SOURCE CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.h)

link_directories(${PROJECT_SOURCE_DIR}/lib)

function(func_post)
    set(DLLS
            "${CMAKE_CURRENT_SOURCE_DIR}/lib/libEGL.dll"
            "${CMAKE_CURRENT_SOURCE_DIR}/lib/libGLES_CM.dll"
            "${CMAKE_CURRENT_SOURCE_DIR}/lib/libGLESv2.dll"
    )
    foreach(DLL ${DLLS})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${DLL}
                $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )
    endforeach ()
endfunction()