cmake_minimum_required(VERSION 3.7)
project(CPPNG_CompositorPrototype)

set(CMAKE_CXX_STANDARD 17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

#add_subdirectory(src/vendor/imgui)
set(LIB_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/src/vendor/) # Lib folder
## Imgui lib
set(imgui_Includes "${LIB_FOLDER}/imgui/"
        ${LIB_FOLDER}/imgui/backends)
add_library(imgui STATIC
        ${LIB_FOLDER}/imgui/imconfig.h
        ${LIB_FOLDER}/imgui/imgui.cpp
        ${LIB_FOLDER}/imgui/imgui.h
        ${LIB_FOLDER}/imgui/imgui_demo.cpp
        ${LIB_FOLDER}/imgui/imgui_draw.cpp
        ${LIB_FOLDER}/imgui/backends/imgui_impl_opengl3.cpp
        ${LIB_FOLDER}/imgui/backends/imgui_impl_opengl3.h
        ${LIB_FOLDER}/imgui/backends/imgui_impl_opengl3_loader.h
        ${LIB_FOLDER}/imgui/backends/imgui_impl_glfw.cpp
        ${LIB_FOLDER}/imgui/backends/imgui_impl_glfw.h
        ${LIB_FOLDER}/imgui/imgui_internal.h
        ${LIB_FOLDER}/imgui/imgui_tables.cpp
        ${LIB_FOLDER}/imgui/imgui_widgets.cpp
        ${LIB_FOLDER}/imgui/imstb_rectpack.h
        ${LIB_FOLDER}/imgui/imstb_textedit.h
        ${LIB_FOLDER}/imgui/imstb_truetype.h
        )

include_directories(${CMAKE_SOURCE_DIR}/src/vendor/glad/include)
include_directories(${CMAKE_SOURCE_DIR}/src/vendor/FileBrowser)
include_directories(${CMAKE_SOURCE_DIR}/src/vendor/stb)

include_directories(${INCLUDE_FOLDER} ${imgui_Includes}) # Include folders
add_executable(${PROJECT_NAME} src/vendor/glad/src/glad.c src/shader.h src/gui.h src/main.cpp src/utils.h)
target_link_libraries(${PROJECT_NAME} glut GL GLU glfw GLEW imgui)


