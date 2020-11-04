cmake_minimum_required(VERSION 3.17)
project(SharkEngine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(ROOTSRC ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_executable(
        main
        src/main.cpp
        src/stdafx.hpp
        src/GlobalPreferences.hpp
        src/util/CLogger.cpp
        src/CLogger.hpp
        src/util/Assets.cpp
        src/Assets.hpp
        src/include/Graphic/IncludeVulkan.h
        src/include/Graphic/VulkanGraphicCore.h
        src/include/Engine/CoreDefine.h
        src/core/Engine/Scene/Manager/EntityIDManager.cpp
        src/include/Engine/Scene/Manager/EntityIDManager.h
        src/include/Engine/CoreTypes.h
        src/include/Engine/Scene/Manager/ComponentArray.hpp
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
        src/core/Engine/Scene/Components/Base/Component.cpp
        src/include/Engine/Scene/Components/Base/Component.h
        src/include/Engine/Audio/SharkAudio.hpp
        src/include/Engine/UniformBufferCamera/Camera.hpp
        src/core/Engine/Scene/CoreStructs.cpp
        src/include/Engine/CoreStructs.h src/InGame/Scenes/TestScene.cpp
        src/InGame/Scenes/TestScene.h
        src/core/Engine/SystemManager/TimeManager.cpp
        src/include/Engine/SystemManager/TimeManager.h
        src/core/Engine/Scene/Components/Transform.cpp
        src/include/Engine/Scene/Components/Transform.h
        src/InGame/GameObjects/TestObject.cpp
        src/InGame/GameObjects/TestObject.h
        src/InGame/Scripts/TestComponent.cpp
        src/InGame/Scripts/TestComponent.h
        src/include/Graphic/VulkanDrawable.h src/core/Graphic/VulkanDrawableManager.cpp src/include/Graphic/VulkanDrawableManager.h src/include/Graphic/VulkanGraphicsCore.cpp src/include/Graphic/VulkanTextureManager.cpp src/include/Graphic/VulkanTextureManager.h src/core/Graphic/DirectX9Core/DirectXCore.cpp src/include/Graphic/DirectX9Core/DirectXCore.h src/core/Graphic/DirectX9Core/WindowsApplication.cpp src/include/Graphic/DirectX9Core/WindowsApplication.h)

target_include_directories(main PRIVATE src)

add_subdirectory(lib/freealut)
target_include_directories(main PRIVATE lib/freealut/include)
target_link_libraries(main PRIVATE alut)

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

find_package(gli CONFIG REQUIRED)
target_link_libraries(main PRIVATE gli)

target_include_directories(main PUBLIC lib/directx/Include)

IF( WIN32 )
    SET( PSDK_PATH "C:/Program Files/Microsoft Platform SDK/Bin" )
    SET( PSDK_INC "C:/Program Files/Microsoft Platform SDK/Include" )

    FIND_PATH(DX9_INCLUDE_PATH d3d9.h
            PATHS
            "$ENV{DXSDK_DIR}/Include"
            "$ENV{PROGRAMFILES}/Microsoft DirectX SDK/Include"
            DOC "The directory where D3D9.h resides")


    FIND_LIBRARY(D3D9_LIBRARY d3d9.lib
            PATHS
            "$ENV{DXSDK_DIR}/Lib/x86"
            "$ENV{PROGRAMFILES}/Microsoft DirectX SDK/Lib/x86"
            DOC "The directory where d3d9.lib resides")

    FIND_LIBRARY(D3DX9_LIBRARY d3dx9.lib
            PATHS
            "$ENV{DXSDK_DIR}/Lib/x86"
            "$ENV{PROGRAMFILES}/Microsoft DirectX SDK/Lib/x86"
            DOC "The directory where d3dx9.lib resides")

    SET( DX9_LIBRARIES ${D3D9_LIBRARY} ${D3DX9_LIBRARY} )

    FIND_LIBRARY( WIN32_COMCTRL comctl32.lib
            PATHS
            "C:/Programme/Microsoft Platform SDK for Windows Server 2003 R2/Lib"
            DOC "Path to psdk"
            )
ENDIF( WIN32 )