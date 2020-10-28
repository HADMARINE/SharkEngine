cmake_minimum_required(VERSION 3.17)
project(SharkEngine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(
        main
        src/main.cpp
        src/stdafx.hpp
        src/GlobalPreferences.hpp
        src/util/CLogger.cpp
        src/CLogger.hpp
        src/util/Assets.cpp
        src/Assets.hpp
        src/core/engine/VulkanEngine.hpp
        src/core/engine/VulkanCore.cpp
        src/include/Engine/CoreDefine.h
        src/core/Engine/Transfrom.cpp
        src/Transfrom.h
        src/core/Engine/Scene/Manager/EntityIDManager.cpp
        src/include/Engine/Scene/Manager/EntityIDManager.h
        src/include/Engine/CoreTypes.h
        src/include/Engine/ComponentArray.hpp
        src/core/Engine/Scene/Manager/ComponentManager.cpp
        src/include/Engine/Scene/Manager/ComponentManager.h
        src/core/Engine/SystemManager/SceneManager.cpp
        src/include/Engine/SystemManager/SceneManager.h
        src/core/Engine/Scene/Entity/Entity.cpp
        src/include/Engine/Scene/Entity/Entity.h
        src/include/Engine/SharkSystem.hpp
        src/include/Engine/SystemManager/InputManager.hpp
        src/core/Engine/Scene/Scene.cpp
        src/include/Engine/Scene/Scene.h
        src/core/Engine/Scene/Manager/SignatureManager.cpp
        src/include/Engine/Scene/Manager/SignatureManager.h
        src/core/Engine/Scene/Components/Base/Component.cpp
        src/include/Engine/Scene/Components/Base/Component.h
        src/include/Engine/Audio/SharkAudio.hpp
)

include_directories("lib/freealut/include")
add_library(Alut OBJECT "./lib/freealut")

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

find_path(STB_INCLUDE_DIRS "stb.h")
target_include_directories(main PRIVATE ${STB_INCLUDE_DIRS})

find_package(tinyobjloader CONFIG REQUIRED)
target_link_libraries(main PRIVATE tinyobjloader::tinyobjloader)

find_package(OpenAL CONFIG REQUIRED)
target_link_libraries(main PRIVATE OpenAL::OpenAL)