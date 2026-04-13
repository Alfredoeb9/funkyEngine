cmake_minimum_required(VERSION 3.14)
project(Bengine CXX)

# Fetch SDL2 and OpenGL stuff here if Bengine needs it independently
# (You might want to move your FetchContent for SDL2 from the game into here!)

set(Bengine_SOURCES
    funkyEngine/core/Errors.cpp
    funkyEngine/core/ResourceManager.cpp
    funkyEngine/core/TextureCache.cpp
    funkyEngine/core/Window.cpp
    funkyEngine/entities/Sprites.cpp
    funkyEngine/graphics/Shaders.cpp
    funkyEngine/graphics/ImageLoader.cpp
    funkyEngine/graphics/GLSLProgram.cpp
    funkyEngine/graphics/IOManager.cpp
    funkyEngine/graphics/picoPNG.cpp

    funkyEngine/FunkyEngine.cpp
)

add_library(Bengine STATIC ${Bengine_SOURCES})

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	0af55ccecd98d4e5a8d1fad7de25ba429d60e863 #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)

# Tell anyone who links to Bengine where to find its header files
target_include_directories(Bengine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/funkyEngine
)

# Link SDL2 and OpenGL to Bengine here
target_link_libraries(Bengine PUBLIC 
    SDL2main 
    SDL2-static
    SDL2_image 
    OpenGL::GL 
    GLEW::GLEW 
    glm::glm
)