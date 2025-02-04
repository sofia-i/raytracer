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
        Camera.h
        main.cpp
        BaseObject.hpp
        PpmWriter.hpp
        Ray.hpp
        Raytracer.cpp
        Raytracer.hpp
        Scene.hpp
        SceneParser.cpp
        SceneParser.hpp
        Sphere.cpp
        Sphere.h
        Triangle.cpp
        Triangle.hpp
        vec3.cpp
        vec3.hpp
        Light.cpp
        Light.h
        Mesh.cpp
        Mesh.h
        Utils.h
        Material.cpp
        Material.h
        Cylinder.cpp
        Cylinder.h
        Geometry.h
        BoundingBox.cpp
        BoundingBox.h
        BoundingVolumeHierarchy.cpp
        BoundingVolumeHierarchy.h
        Extent.h)
