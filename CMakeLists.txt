cmake_minimum_required(VERSION 3.15)

project(lo21 VERSION 1.0 LANGUAGES CXX)

# Standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# Met l'exécutable dans build/bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Inclure les headers
include_directories(${CMAKE_SOURCE_DIR}/include)

# Récupérer tous les .cpp du dossier src
file(GLOB_RECURSE LO21_SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

set(LO21_HEADERS
    "${CMAKE_SOURCE_DIR}/include/mainwindow.h"
    "${CMAKE_SOURCE_DIR}/include/TuileItem.h"
    "${CMAKE_SOURCE_DIR}/include/PageWidget.h"
)

# Créer l'exécutable
add_executable(lo21 ${LO21_SOURCES} ${LO21_HEADERS}
    include/PageWidget.h include/mainwindow.h include/TuileItem.h include/JeuController.h include/HexItem.h include/Sauvegarde.h include/Position.h include/ControleConsole.h include/Tuile.h include/Plateau.h include/Partie.h include/Joueur.h include/Hexagone.h include/Chantier.h include/couleurs_console.h
    src/mainWindow.cpp src/QTlancerPartieWidget.cpp src/PageWidget.cpp src/TuileItem.cpp src/JeuController.cpp src/HexItem.cpp src/main.cpp src/Plateau.cpp src/Partie.cpp src/Joueur.cpp src/ControleConsole.cpp src/Sauvegarde.cpp src/Tuile.cpp src/Chantier.cpp src/Hexagone.cpp
    saves
)
target_link_libraries(lo21 PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

# Options de warnings (confort)
if(MSVC)
    target_compile_options(lo21 PRIVATE /W4 /permissive-)
else()
    target_compile_options(lo21 PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Copie des ressources (images) à côté de l'exécutable pour garantir leur disponibilité
add_custom_target(copy_images ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/img
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/img
    COMMENT "Copie des images dans le dossier d'exécution"
)
add_dependencies(lo21 copy_images)
