cmake_minimum_required(VERSION 3.5.0)
project(RayTracing VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)  # 设置 C++ 标准，根据需要修改
set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_MAKE_PROGRAM "make")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_COMPILER "gcc")

# 添加源文件
set(SOURCES
    Camera.cpp
    Color.cpp
    Material.cpp
    Random.cpp
    Ray.cpp
    RayTracing.cpp
    Scene.cpp
    Sphere.cpp
)


# 添加可执行文件
add_executable(RayTracing ${SOURCES})

include(CTest)
enable_testing()
