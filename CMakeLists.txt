cmake_minimum_required(VERSION 3.15)

project(lo21 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt (auto MOC/UIC/RCC)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# Sortie dans build/bin/<CONFIG> (Debug/Release/...)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/bin/RelWithDebInfo)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${CMAKE_BINARY_DIR}/bin/MinSizeRel)

# Récupération automatique des fichiers (avec des *)
file(GLOB_RECURSE LO21_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/srcView/*.cpp"
)

file(GLOB_RECURSE LO21_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/include/*.h"
    "${CMAKE_SOURCE_DIR}/include/*.hpp"
    "${CMAKE_SOURCE_DIR}/IncludeView/*.h"
    "${CMAKE_SOURCE_DIR}/IncludeView/*.hpp"
)

# Si vous avez des .ui ou .qrc, on les récupère aussi
file(GLOB_RECURSE LO21_FORMS CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/srcView/*.ui"
    "${CMAKE_SOURCE_DIR}/IncludeView/*.ui"
)

file(GLOB_RECURSE LO21_RESOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/*.qrc"
)

add_executable(lo21
    ${LO21_SOURCES}
    ${LO21_HEADERS}
    ${LO21_FORMS}
    ${LO21_RESOURCES}
)

target_include_directories(lo21 PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/IncludeView
)

target_link_libraries(lo21 PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

# Options de warnings (confort)
if(MSVC)
    target_compile_options(lo21 PRIVATE /W4 /permissive-)
else()
    target_compile_options(lo21 PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Copie des images à côté de l'exécutable réel (Debug/Release/...)
add_custom_command(TARGET lo21 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/img
            $<TARGET_FILE_DIR:lo21>/img
    COMMENT "Copie des images à côté de l'exécutable"
)
