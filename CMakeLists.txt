cmake_minimum_required(VERSION 3.0.0)
project(magnumTest VERSION 0.1.0)

INCLUDE_DIRECTORIES(
    c:/dev/vcpkg/installed/x64-windows/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

LINK_DIRECTORIES(
    C:/dev/vcpkg/installed/x64-windows/lib
)

find_package(Corrade REQUIRED Main)
find_package(Magnum CONFIG REQUIRED 
    GL
    MeshTools
    Primitives
    Shaders
    Sdl2Application
)

find_package(Catch2 CONFIG REQUIRED)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

add_executable(magnumTest src/PrimitivesExample.cpp)

target_link_libraries(magnumTest
    Corrade::Main
    Magnum::Magnum
    Magnum::GL
    Magnum::Application
    Magnum::Shaders
    Magnum::MeshTools
    Magnum::Primitives
)

include(CTest)
include(Catch)

# We check if this is the main file
# you don't usually want users of your library to
# execute tests as part of their build
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    add_subdirectory(tests)
endif ()


ENABLE_TESTING()