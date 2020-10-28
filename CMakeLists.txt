cmake_minimum_required(VERSION 3.17)
project(SharkEngine VERSION 1.0 LANGUAGES CXX)

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
        src/core/engine/VulkanApplication.cpp
        src/include/Graphic/VulkanApplication.h
        src/core/engine/VulkanDevice.cpp
        src/include/Graphic/VulkanDevice.h
        src/core/engine/VulkanDescriptor.cpp
        src/include/Graphic/VulkanDescriptor.h
        src/core/engine/VulkanDrawable.cpp
        src/include/Graphic/VulkanDrawable.h
        src/core/engine/VulkanInstance.cpp
        src/include/Graphic/VulkanInstance.h
        src/core/engine/VulkanPipeline.cpp
        src/include/Graphic/VulkanPipeline.h
        src/core/engine/VulkanRenderer.cpp
        src/include/Graphic/VulkanRenderer.h
        src/core/engine/VulkanShader.cpp
        src/include/Graphic/VulkanShader.h
        src/core/engine/VulkanSwapChain.cpp
        src/include/Graphic/VulkanSwapChain.h
        src/core/engine/VulkanLayer.cpp src/include/Graphic/VulkanLayer.h src/include/Graphic/IncludeVulkan.h src/core/engine/VulkanStructs.cpp src/include/Graphic/VulkanStructs.h src/core/engine/VulkanMeshStruct.cpp src/include/Graphic/VulkanMeshStruct.h)

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
