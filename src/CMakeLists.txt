add_executable(${PROJECT_NAME} main.cpp
        private/aabb.cpp
        private/bvh.cpp
        private/camera.cpp
        private/material.cpp
)

target_include_directories(${PROJECT_NAME}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/public
        INTERFACE stb)