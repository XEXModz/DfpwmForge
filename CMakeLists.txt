cmake_minimum_required(VERSION 3.20)

project(DfpwmForge VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(dfpwm
        src/main.cpp
        src/mp3_decoder.cpp
        src/audio_processor.cpp
        src/dfpwm_encoder.cpp
)

target_include_directories(dfpwm PRIVATE
        external
        include
)

if(MSVC)
    target_compile_options(dfpwm PRIVATE /W4)
else()
    target_compile_options(dfpwm PRIVATE -Wall -Wextra -Wpedantic)
endif()

set_target_properties(dfpwm PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
