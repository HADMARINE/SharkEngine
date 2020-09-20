cmake_minimum_required(VERSION 3.17)
project(vulkan_engine VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(
    vulkan_engine
    src/main.cpp
    src/stdafx.hpp
    src/global_prefences.hpp
    src/util/CLogger.cpp
    src/CLogger.hpp
    src/util/Assets.cpp
    src/Assets.hpp
)

include_directories(
    $ENV{VCPKG_INSTALLED_DIR}/include
    $ENV{VULKAN_SDK}/include
)

message($ENV{VULKAN_SDK}/include/vulkan)

find_package(glm CONFIG REQUIRED)
target_link_libraries(vulkan_engine PRIVATE glm)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(vulkan_engine PRIVATE glfw)

if(WIN32 OR WIN64)
  message(WINDOWS_NOT_READY_YET)
  find_library(
      LIBVULKAN
      NAMES libvulkan.dll
      HINTS $ENV{VULKAN_SDK}/lib
      REQUIRED
  )
else()
  find_library(
      LIBVULKAN
      NAMES libvulkan.dylib
      HINTS $ENV{VULKAN_SDK}/lib
      REQUIRED
  )
endif()

target_link_libraries(vulkan_engine PRIVATE ${LIBVULKAN})

find_library(
    LIBJSONCPP
    NAMES libjsoncpp.a
    HINTS $ENV{VCPKG_INSTALLED_DIR}/lib
    REQUIRED
)

target_link_libraries(vulkan_engine PRIVATE ${LIBJSONCPP})