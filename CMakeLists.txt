cmake_minimum_required(VERSION 3.29)
project(HyperGpu)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(EXTERNAL "${CMAKE_SOURCE_DIR}/external/")
set(INTERFACE "${CMAKE_SOURCE_DIR}/include/")
set(INTERNAL_HEADER ${CMAKE_SOURCE_DIR}/src/include/)

find_package(Vulkan REQUIRED)
add_subdirectory(${EXTERNAL}/spdlog)

include_directories(${EXTERNAL} ${INTERFACE} ${INTERNAL_HEADER})

list(APPEND EXTERNAL_SOURCE
    ${EXTERNAL}/SPIRV-Reflect/spirv_reflect.c
)

file(GLOB_RECURSE HEADER CONFIGURE_DEPENDS ${INTERFACE}/*.h ${INTERNAL_HEADER}/*.h)
file(GLOB_RECURSE SOURCE CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/source/*.cpp)

add_library(${PROJECT_NAME} SHARED ${HEADER} ${SOURCE} ${EXTERNAL_SOURCE})

target_include_directories(${PROJECT_NAME} PRIVATE ${Vulkan_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog ${Vulkan_LIBRARIES})

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    include(${CMAKE_SOURCE_DIR}/cmake/windows.cmake)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    include(${CMAKE_SOURCE_DIR}/cmake/macos.cmake)
endif ()