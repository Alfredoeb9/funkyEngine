cmake_minimum_required(VERSION 3.14)
project(Bengine CXX)

# set(SDL2_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
# set(SDL2_BUILD_STATIC_LIBS ON CACHE BOOL "" FORCE)
# set(SDL2IMAGE_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
# set(SDL2IMAGE_BUILD_STATIC_LIBS ON CACHE BOOL "" FORCE)

# Fetch SDL2 and OpenGL stuff here if Bengine needs it independently
# (You might want to move your FetchContent for SDL2 from the game into here!)
# include(FetchContent)

# # 1. Fetch SDL2
# FetchContent_Declare(
#     SDL2
#     GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
#     GIT_TAG release-2.28.5
# )
# FetchContent_MakeAvailable(SDL2)

# # 2. Fetch SDL2_image
# FetchContent_Declare(
#     SDL2_image
#     GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
#     GIT_TAG release-2.8.2
# )
# FetchContent_MakeAvailable(SDL2_image)

# # 3. Fetch GLM
# FetchContent_Declare(
#     glm
#     GIT_REPOSITORY	https://github.com/g-truc/glm.git
#     GIT_TAG 	0af55ccecd98d4e5a8d1fad7de25ba429d60e863 #refs/tags/1.0.1
# )
# FetchContent_MakeAvailable(glm)
set(SDL2_ttf_DIR "C:/Libraries/SDL2_ttf-2.24.0/x86_64-w64-mingw32/lib/cmake/SDL2_ttf")
set(SDL2_mixer_DIR "C:/Libraries/SDL2_mixer-2.8.1/x86_64-w64-mingw32/lib/cmake/SDL2_mixer")# 4. Find System Packages (Must be called BEFORE linking targets like OpenGL::GL)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(glm REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

set(Bengine_SOURCES
    funkyEngine/core/Camera2D.cpp
    funkyEngine/core/Errors.cpp
    funkyEngine/core/ResourceManager.cpp
    funkyEngine/core/TextureCache.cpp
    funkyEngine/core/Window.cpp
    funkyEngine/core/InputManager.cpp
    funkyEngine/core/Timing.cpp
    funkyEngine/entities/Sprites.cpp
    funkyEngine/entities/SpriteFont.cpp
    funkyEngine/entities/SpriteBatch.cpp
    funkyEngine/graphics/ImageLoader.cpp
    funkyEngine/graphics/GLSLProgram.cpp
    funkyEngine/graphics/IOManager.cpp
    funkyEngine/graphics/picoPNG.cpp
    funkyEngine/FunkyEngine.cpp
    funkyEngine/core/Logger.cpp
    funkyEngine/core/AudioEngine.cpp
)

add_library(Bengine STATIC ${Bengine_SOURCES}
    funkyEngine/core/Logger.h
    funkyEngine/core/Logger.cpp
    funkyEngine/core/AudioEngine.h
    funkyEngine/core/ParticleEngine2D.h
    funkyEngine/core/ParticleBatch2D.h)
add_library(FunkyEngine ALIAS Bengine)

# Tell anyone who links to Bengine where to find its header files
target_include_directories(Bengine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/funkyEngine
)

# Link SDL2 and OpenGL to Bengine here
target_link_libraries(Bengine PUBLIC 
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
    SDL2_mixer::SDL2_mixer
    OpenGL::GL 
    GLEW::GLEW 
    glm::glm
)