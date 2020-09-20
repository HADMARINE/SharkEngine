cmake_minimum_required(VERSION 3.17)
project(vulkan_engine)

set(CMAKE_CXX_STANDARD 17)

add_executable(vulkan_engine
               src/main.cpp
               src/stdafx.hpp
               src/global_prefences.hpp
               src/util/CLogger.cpp
               src/CLogger.hpp
               src/util/Assets.cpp
               src/Assets.hpp
               )