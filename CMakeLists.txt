cmake_minimum_required(VERSION 3.15)

project(lo21 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# Met l'exécutable dans build/bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Récupérer tous les .cpp du dossier src
file(GLOB_RECURSE LO21_SOURCES
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_SOURCE_DIR}/srcView/*.cpp"
)

set(LO21_HEADERS
    "${CMAKE_SOURCE_DIR}/IncludeView/mainwindow.h"
    "${CMAKE_SOURCE_DIR}/IncludeView/TuileItem.h"
    "${CMAKE_SOURCE_DIR}/IncludeView/PageWidget.h"
    "${CMAKE_SOURCE_DIR}/IncludeView/PlateauWidget.h"
    "${CMAKE_SOURCE_DIR}/IncludeView/EchapWidget.h"
    "${CMAKE_SOURCE_DIR}/IncludeView/ChantierWidget.h"
    "${CMAKE_SOURCE_DIR}/IncludeView/ZoneJeuWidget.h"
    "${CMAKE_SOURCE_DIR}/IncludeView/ScorePanel.h"
    "${CMAKE_SOURCE_DIR}/IncludeView/HexItem.h"
    "${CMAKE_SOURCE_DIR}/IncludeView/WidgetUtilitaire.h"
)

# Créer l'exécutable
add_executable(lo21 ${LO21_SOURCES} ${LO21_HEADERS}
    include/Chantier.h include/ControleConsole.h include/couleurs_console.h include/Hexagone.h include/Joueur.h include/JoueurFactory.h include/Partie.h include/Plateau.h include/Position.h include/Sauvegarde.h include/Tuile.h include/TuileGenerator.h include/TuileGeneratorFactory.h IncludeView/ChantierWidget.h IncludeView/ControllerView.h IncludeView/EchapWidget.h IncludeView/HexItem.h IncludeView/mainwindow.h IncludeView/PageWidget.h IncludeView/PlateauWidget.h IncludeView/ScorePanel.h IncludeView/TuileItem.h IncludeView/WidgetUtilitaire.h IncludeView/ZoneJeuWidget.h src/Chantier.cpp src/ControleConsole.cpp src/Hexagone.cpp src/Joueur.cpp src/JoueurFactory.cpp src/Partie.cpp src/Plateau.cpp src/Tuile.cpp src/TuileGenerator.cpp src/TuileGeneratorFactory.cpp srcView/ChantierWidget.cpp srcView/ControllerView.cpp srcView/EchapWidget.cpp srcView/HexItem.cpp srcView/main.cpp srcView/mainWindow.cpp srcView/PageWidget.cpp srcView/PlateauWidget.cpp srcView/Sauvegarde.cpp srcView/ScorePanel.cpp srcView/TuileItem.cpp srcView/WidgetUtilitaire.cpp srcView/ZoneJeuWidget.cpp)
target_include_directories(lo21 PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/IncludeView
    $<TARGET_PROPERTY:Qt6::Core,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:Qt6::Gui,INTERFACE_INCLUDE_DIRECTORIES>
    $<TARGET_PROPERTY:Qt6::Widgets,INTERFACE_INCLUDE_DIRECTORIES>
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
