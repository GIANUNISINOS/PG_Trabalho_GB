cmake_minimum_required(VERSION 3.10)
project(PROJETO_PG_GB)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(PROJETO_PG_GB
        CMakeLists.txt
        source/main.cpp
        header/Includes.h
        header/Shader.h
        header/Texture.h
        header/Layer.h
        header/SpriteSheet.h
        header/VerticesObject.h
        header/Transformations.h
        header/GameObject.h
        header/Tile.h
        header/Tilemap.h
        )

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_library(Cocoa REQUIRED)
find_library(CoreVideo REQUIRED)
find_library(IOKit REQUIRED)
find_package(GLFW3 REQUIRED)

##/usr/local/lib/libglfw.3.2.dylib

target_link_libraries(PROJETO_PG_GB /usr/local/lib/libglfw.3.3.dylib ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})
target_include_directories(PROJETO_PG_GB PRIVATE ${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} /usr/local/include/glm)
