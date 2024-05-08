cmake_minimum_required(VERSION 3.28)
project(raytracinginoneweekend)

set(CMAKE_CXX_STANDARD 20)

add_executable(raytracinginoneweekend main.cpp
        vec3.h
        color.h
        ray.h
        hittable.h
        sphere.h
        hittable_list.h
        includes.h
        interval.h
        camera.h
        material.h)
