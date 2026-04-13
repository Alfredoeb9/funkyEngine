cmake_minimum_required(VERSION 3.14)
project(Bengine CXX)

# Fetch SDL2 and OpenGL stuff here if Bengine needs it independently
# (You might want to move your FetchContent for SDL2 from the game into here!)

set(Bengine_SOURCES
    engine/core/Errors.cpp
    engine/core/ResourceManager.cpp
    engine/core/TextureCache.cpp
    engine/entities/Sprites.cpp
    engine/graphics/Shaders.cpp
    engine/graphics/ImageLoader.cpp
    engine/graphics/GLSLProgram.cpp
    engine/graphics/IOManager.cpp
    engine/graphics/picoPNG.cpp
)

add_library(Bengine STATIC ${Bengine_SOURCES})

# Tell anyone who links to Bengine where to find its header files
target_include_directories(Bengine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/engine
)

# Link SDL2 and OpenGL to Bengine here
target_link_libraries(Bengine PUBLIC SDL2main SDL2-static OpenGL::GL GLEW::GLEW)