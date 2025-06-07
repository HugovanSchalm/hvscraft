cmake_minimum_required(VERSION 3.10)

project(Craft VERSION 0.1)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(OpenGL_GL_PREFERENCE "GLVND")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(ASSETS_LOCATION "${PROJECT_SOURCE_DIR}/assets")
set(SHADERS_LOCATION "\"${ASSETS_LOCATION}/shaders\"")
set(TEXTURES_LOCATION "\"${ASSETS_LOCATION}/textures\"")
configure_file(assets.h.in assets.h)

# set the project name and version
project(Craft VERSION 1.0)


file(GLOB Sourcefiles src/*.c)
add_executable(Craft ${Sourcefiles})

target_include_directories(Craft PUBLIC 
    "${PROJECT_SOURCE_DIR}/include",
    "${PROJECT_BINARY_DIR}"
)

add_subdirectory(lib/glfw)
target_link_libraries(Craft PUBLIC glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(Craft PUBLIC OpenGL::GL)



# target_include_directories(Tutorial PUBLIC
# "${PROJECT_BINARY_DIR}"
#"${PROJECT_SOURCE_DIR}/MathFunctions"
#)
