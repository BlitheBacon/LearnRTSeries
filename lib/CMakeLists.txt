add_library(stb INTERFACE
        stb/stb_image.h)

target_include_directories(stb
        INTERFACE
        "${PROJECT_SOURCE_DIR}/stb")

target_link_libraries(stb INTERFACE stb)