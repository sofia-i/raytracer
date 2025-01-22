cmake_minimum_required(VERSION 3.29)
project(raytracer_2)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

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
        Material.h)
