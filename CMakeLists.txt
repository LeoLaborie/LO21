cmake_minimum_required(VERSION 3.15)

project(lo21 VERSION 1.0 LANGUAGES CXX)

# Standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Met l'exécutable dans build/bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Inclure les headers
include_directories(${CMAKE_SOURCE_DIR}/include)

# Récupérer tous les .cpp du dossier src
file(GLOB_RECURSE LO21_SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

# Créer l'exécutable
add_executable(lo21 ${LO21_SOURCES})

# Options de warnings (confort)
if(MSVC)
    target_compile_options(lo21 PRIVATE /W4 /permissive-)
else()
    target_compile_options(lo21 PRIVATE -Wall -Wextra -Wpedantic)
endif()
