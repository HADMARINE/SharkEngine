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
        src/include/Engine/SystemManager/InputManager.h
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
        src/include/Graphic/VulkanDrawable.h
        src/core/Graphic/VulkanDrawableManager.cpp
        src/include/Graphic/VulkanDrawableManager.h
        src/include/Graphic/VulkanGraphicsCore.cpp
        src/include/Graphic/VulkanTextureManager.cpp
        src/include/Graphic/VulkanTextureManager.h
        src/core/Graphic/DirectX9Core/DirectXCore.cpp
        src/include/Graphic/DirectX9Core/DirectXCore.h
        src/core/Graphic/DirectX9Core/WindowsApplication.cpp
        src/include/Graphic/DirectX9Core/WindowsApplication.h
        src/core/Engine/Scene/Components/Sprite2DRenderer.cpp
        src/include/Engine/Scene/Components/Sprite2DRenderer.h
        src/core/Graphic/DirectX9Core/DirectXTextureManger.cpp
        src/include/Engine/SystemManager/DirectXTextureManger.h
        src/include/Engine/Scene/GameObject/GameObject.cpp
        src/include/Engine/Scene/GameObject/GameObject.h
        src/core/Engine/Scene/Components/SpriteAnimation.cpp
        src/include/Engine/Scene/Components/SpriteAnimation.h
        src/core/Engine/SystemManager/InputManager.cpp
        src/core/Engine/Scene/Components/Transform.cpp
        src/include/Engine/Scene/Components/Transform.h
        src/InGame/GameObjects/TestObject.cpp
        src/InGame/GameObjects/TestObject.h
        src/InGame/Scripts/TestComponent.cpp
        src/InGame/Scripts/TestComponent.h
        src/include/Engine/Physics/Physics2D.hpp
        src/include/Engine/Physics/RigidBody2D.hpp
        src/include/Graphic/VulkanDrawable.h
        src/core/Graphic/VulkanDrawableManager.cpp
        src/include/Graphic/VulkanDrawableManager.h
        src/include/Graphic/VulkanGraphicsCore.cpp
        src/include/Graphic/VulkanTextureManager.cpp
        src/include/Graphic/VulkanTextureManager.h
        src/core/Graphic/DirectX9Core/DirectXCore.cpp
        src/include/Graphic/DirectX9Core/DirectXCore.h
        src/core/Graphic/DirectX9Core/WindowsApplication.cpp
        src/include/Graphic/DirectX9Core/WindowsApplication.h
        src/core/Engine/Scene/Components/Sprite2DRenderer.cpp
        src/include/Engine/Scene/Components/Sprite2DRenderer.h
        src/core/Graphic/DirectX9Core/DirectXTextureManger.cpp
        src/include/Engine/SystemManager/DirectXTextureManger.h
        src/include/Engine/Scene/GameObject/GameObject.cpp
        src/include/Engine/Scene/GameObject/GameObject.h
        src/core/Engine/Scene/Components/SpriteAnimation.cpp
        src/include/Engine/Scene/Components/SpriteAnimation.h
        src/core/Engine/SystemManager/InputManager.cpp
        src/core/Engine/SystemManager/LineDebuggerManager.cpp
        src/include/Engine/SystemManager/LineDebuggerManager.h
        src/core/Engine/SystemManager/ColliderManager.cpp
        src/include/Engine/Scene/Manager/ColliderManager.hpp
        src/include/Engine/Scene/Components/RigidBody2D.cpp
        src/include/Engine/Scene/Components/RigidBody2D.hpp
        src/include/Engine/Scene/Components/CameraComponent.h
        src/core/Engine/Scene/Components/CameraComponent.cpp
        src/include/Engine/Scene/Components/BoxCollider.hpp
        src/core/Engine/Scene/Components/BoxCollider.cpp)

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
target_link_libraries(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/directx/Lib/d3dx9.lib)
target_link_libraries(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/directx/Lib/dxguid.lib)
target_link_libraries(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/directx/Lib/d3dxof.lib)
target_link_libraries(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/directx/Lib/d3d9.lib)

##---------------------------------------------------------------------------------
#message("Looking for DirectX9...")
#
#set(DXSDK_DIR "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/")
#set(DIRECTX_ROOT_DIR "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/")
#
#file(GLOB DX9_SEARCH_PATHS
#        "$ENV{DXSDK_DIR}/Include"
#        "$ENV{PROGRAMFILES}/Microsoft DirectX SDK (June 2010)/Include"
#        "$ENV{PROGRAMFILES}/Microsoft SDKs/Windows/*/Include"
#        "C:/Program Files (x86)/Windows Kits/*/include/um"
#        "C:/Program Files/Windows Kits/*/include/um"
#        "C:/Program Files (x86)/Windows Kits/10/Include/10.0.*.0/um"
#        "C:/Program Files/Windows Kits/10/Include/10.0.*.0/um"
#        )
#
#find_path(DIRECTX_INCLUDE_DIRS
#        NAMES d3d9.h
#        PATHS ${DX9_SEARCH_PATHS})
#
#if(CMAKE_SIZEOF_VOID_P EQUAL 8)
#    SET(DX9_LIBRARY_PATHS
#            "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include"
#            "C:/Program Files (x86)/Windows Kits/8.1/Lib/winv6.3/um/x64/"
#            "C:/Program Files (x86)/Windows Kits/8.0/Lib/win8/um/x64/"
#            "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.14393.0/um/x64/"
#            "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.10586.0/um/x64/"
#            "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.15063.0/um/x64/"
#            "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.16299.0/um/x64/"
#            "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.19041.0/um/x64/"
#            )
#else()
#    SET(DX9_LIBRARY_PATHS
#            "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86/"
#            "C:/Program Files (x86)/Windows Kits/8.1/lib/winv6.3/um/x86/"
#            "C:/Program Files (x86)/Windows Kits/8.0/lib/win8/um/x86/"
#            "C:/Program Files (x86)/Windows Kits/10/lib/10.0.14393.0/um/x86/"
#            "C:/Program Files (x86)/Windows Kits/10/lib/10.0.10586.0/um/x86/"
#            "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.15063.0/um/x86/"
#            "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.16299.0/um/x86/"
#            "C:/Program Files (x86)/Windows Kits/10/Lib/10.0.19041.0/um/x86/"
#            )
#endif()
#
#find_library(DX9_LIB d3dx9 ${DX9_LIBRARY_PATHS} NO_DEFAULT_PATH)
#set(DIRECTX_LIBRARIES ${DX9_LIB})
#
#message("Include: ${DIRECTX_INCLUDE_DIRS}")
#message("Libs: ${DIRECTX_LIBRARIES}")
#
#include(FindPackageHandleStandardArgs)
#FIND_PACKAGE_HANDLE_STANDARD_ARGS(DIRECTX9
#        DEFAULT_MSG DIRECTX_ROOT_DIR
#        DIRECTX_INCLUDE_DIRS DIRECTX_LIBRARIES
#        )
#mark_as_advanced(DIRECTX_INCLUDE_DIRS DX9_LIB)
#
#target_include_directories(main PRIVATE ${DXSDK_DIR})
#message("${DXSDK_DIR}")