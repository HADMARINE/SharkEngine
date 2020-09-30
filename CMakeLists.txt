cmake_minimum_required(VERSION 3.17)
project(vulkan_engine VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(
        main
        src/main.cpp
        src/stdafx.hpp
        src/global_preferences.hpp
        src/util/CLogger.cpp
        src/CLogger.hpp
        src/util/Assets.cpp
        src/Assets.hpp
        src/HadmarineEngine.hpp
        src/core/Engine/GameObject.cpp src/GameObject.h src/core/Engine/Component.cpp src/Component.h src/Object.h src/core/Engine/Scene.cpp src/Scene.h src/core/Engine/Manager/SceneManager.cpp src/SceneManager.h src/core/Engine/Manager/ResourcesManager.cpp src/ResourcesManager.h src/core/Engine/Message.cpp src/Message.h)

find_package(glm CONFIG REQUIRED)
target_link_libraries(main PRIVATE glm)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(main PRIVATE glfw)

find_package(Vulkan REQUIRED)
target_include_directories(main PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(main PRIVATE Vulkan::Vulkan)

find_package(jsoncpp REQUIRED)
get_target_property(JSON_INC_PATH jsoncpp_lib INTERFACE_INCLUDE_DIRECTORIES)
target_include_directories(main PUBLIC ${Jsoncpp_INCLUDE_DIRS})
target_link_libraries(main PRIVATE jsoncpp_lib)