cmake_minimum_required(VERSION 3.29)
project(HyperGpu)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(EXTERNAL "${PROJECT_SOURCE_DIR}/external")
set(INTERFACE "${PROJECT_SOURCE_DIR}/include")

find_package(Vulkan REQUIRED)
add_subdirectory(${EXTERNAL}/spdlog)

list(APPEND EXTERNAL_SOURCE
    ${EXTERNAL}/SPIRV-Reflect/spirv_reflect.c
)

include_directories(${Vulkan_INCLUDE_DIR} ${EXTERNAL})

file(GLOB_RECURSE SOURCE CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cpp ${PROJECT_SOURCE_DIR}/src/*.h)

add_library(${PROJECT_NAME} SHARED ${SOURCE} ${EXTERNAL_SOURCE})

target_link_libraries(${PROJECT_NAME} PUBLIC spdlog::spdlog ${Vulkan_LIBRARIES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INTERFACE})

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/windows.cmake)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/macos.cmake)
endif ()