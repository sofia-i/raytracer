cmake_minimum_required(VERSION 3.29)
project(raytracer_2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

# PROFILING SETTINGS
set(PROFILE 0)
if(PROFILE)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
endif()

add_executable(raytracer_2
        src/Camera.h
        src/main.cpp
        src/PpmWriter.hpp
        src/Ray.hpp
        src/Raytracer.cpp
        src/Raytracer.hpp
        src/Scene.hpp
        src/SceneParser.cpp
        src/SceneParser.hpp
        src/Sphere.cpp
        src/Sphere.h
        src/Triangle.cpp
        src/Triangle.hpp
        src/vec3.cpp
        src/vec3.hpp
        src/Light.cpp
        src/Light.h
        src/Mesh.cpp
        src/Mesh.h
        src/Utils.h
        src/Material.cpp
        src/Material.h
        src/Cylinder.cpp
        src/Cylinder.h
        src/Geometry.h
        src/Extent.h
        src/Hittable.h
        src/BoundingBox.cpp
        src/BoundingVolumeHierarchy.cpp)
