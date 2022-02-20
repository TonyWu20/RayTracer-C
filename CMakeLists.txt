if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)
cmake_minimum_required(VERSION 3.20)
project (RayTracer-C)
set(CMAKE_OSX_DEPLOYMENT_TARGET 12.1)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set (CMAKE_C_FLAGS "-Wall -O3 -I'${PROJECT_BINARY_DIR}'/src")
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
# Test 1
add_library(cannon ./src/cannon/cannon.c)
add_executable(cannon_test ./src/cannon_test.c)
target_link_libraries(cannon_test cannon)
# Test 2
add_executable(clock_test ./src/clock_test.c)
# Test 3
add_library(ray ./src/ray/ray.c)
add_executable(ray_sphere ./src/ray_test.c)
target_link_libraries(ray_sphere ray -lncurses)
# Test 4
add_executable(normal_test ./src/normal_test.c)
