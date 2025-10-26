cmake_minimum_required(VERSION 3.10.0)
project(RISCV_Simulator VERSION 0.1.0 LANGUAGES C)


set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)


include_directories(${PROJECT_SOURCE_DIR}/src)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.c)

# Create the executable
add_executable(${PROJECT_NAME} ${SRC_FILES})
