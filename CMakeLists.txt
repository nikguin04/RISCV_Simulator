cmake_minimum_required(VERSION 3.10.0)
project(RISCV_Simulator VERSION 0.1.0 LANGUAGES C)

include(CTest)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)


include_directories(${PROJECT_SOURCE_DIR}/src)

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.c)

add_library(riscv_lib ${SRC_FILES})

# Create the executable
#add_executable(${PROJECT_NAME} ${SRC_FILES})
add_executable(${PROJECT_NAME} src/main.c)
target_link_libraries(${PROJECT_NAME} PRIVATE riscv_lib)


# TESTING

enable_testing()
add_executable(TestInstantiator tests/tester.c)
target_link_libraries(TestInstantiator PRIVATE riscv_lib)
