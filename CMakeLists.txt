if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)
cmake_minimum_required(VERSION 3.20)
project (RayTracer-C)
set(CMAKE_OSX_DEPLOYMENT_TARGET 12.1)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set (CMAKE_C_FLAGS "-Wall -O3 -I'${PROJECT_BINARY_DIR}'/src -I/opt/homebrew/include")
# set (CMAKE_C_FLAGS "-Wall -g3 -I'${PROJECT_BINARY_DIR}'/src -I/opt/homebrew/include -fsanitize=address -fsanitize=bounds")
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
# Test 1
# add_library(cannon ./src/cannon/cannon.c)
# add_executable(cannon_test ./src/cannon_test.c)
# target_link_libraries(cannon_test cannon)
# Test 2
# add_executable(clock_test ./src/clock_test.c)
# Test 3
# add_library(ray ./src/ray/ray.c)
add_library(shape INTERFACE ./src/geometry/shape.h)
add_library(geometry ./src/geometry/sphere.c ./src/geometry/plane.c ./src/geometry/cube.c ./src/geometry/geometry.c ./src/geometry/patterns.c)
target_link_libraries(geometry shape)
# add_library(geometry ./src/geometry/sphere.c  ./src/geometry/geometry.c)
# add_executable(ray_sphere ./src/ray_test.c)
# target_link_libraries(ray_sphere ray -lncurses)
# Test 4
# add_executable(normal_test ./src/normal_test.c)
# target_link_libraries(normal_test ray)
# # Test lighting
# # add_executable(light_test ./src/light_test.c)
# # target_link_libraries(light_test ray)
# # Test world
# add_executable(world_test ./src/world_test.c)
# target_link_libraries(world_test ray)
# # Test view
# add_executable(view_test ./src/view_test.c)
# target_link_libraries(view_test ray)
# # Test render world 1
add_executable(render_world_test ./src/render_world_test.c)
# target_compile_options(render_world_test PRIVATE -I/opt/homebrew/include -I"${PROJECT_BINARY_DIR}"/src -Xpreprocessor -fopenmp)
target_link_libraries(render_world_test geometry)
# target_link_libraries(render_world_test geometry)
# Test shadow
# add_executable(shadow_test ./src/shadow_test.c)
# target_link_libraries(shadow_test ray)
# Test shape
add_executable(shape_test ./src/shape_test.c)
target_link_libraries(shape_test geometry)
# Test pattern
add_executable(pattern_test ./src/pattern_test.c)
target_link_libraries(pattern_test geometry)
# Test reflection
add_executable(reflect_test ./src/reflect_test.c)
target_link_libraries(reflect_test geometry)
# Test refraction
add_executable(refract_test ./src/refractive_test.c)
target_link_libraries(refract_test geometry)
# Test cube
add_executable(cube_test ./src/cube_test.c)
target_link_libraries(cube_test geometry)
