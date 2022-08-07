cmake_minimum_required(VERSION 3.7)
project(CPPNG_MixturePrototype)

set(CMAKE_CXX_STANDARD 17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")
message(CMAKE_SOURCE_DIR)
include_directories(${CMAKE_SOURCE_DIR}/src/vendor/glad/include)
include_directories(${CMAKE_SOURCE_DIR}/src/vendor/stb)

add_executable(${PROJECT_NAME} src/vendor/glad/src/glad.c src/main.cpp src/shader.h)
target_link_libraries(${PROJECT_NAME} glut GL glfw GLEW)


