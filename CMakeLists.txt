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
)

find_package(glm CONFIG REQUIRED)
target_link_libraries(vulkan_engine PRIVATE glm)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(vulkan_engine PRIVATE glfw)

find_package(Vulkan REQUIRED)
target_include_directories(vulkan_engine PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(vulkan_engine PRIVATE Vulkan::Vulkan)

find_package(jsoncpp REQUIRED)
get_target_property(JSON_INC_PATH jsoncpp_lib INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(vulkan_engine PUBLIC ${Jsoncpp_INCLUDE_DIRS})
target_link_libraries(vulkan_engine PRIVATE jsoncpp_lib)