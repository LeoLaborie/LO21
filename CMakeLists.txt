cmake_minimum_required(VERSION 3.15)
project(lo21 VERSION 1.0 LANGUAGES CXX)

# ============================================================
# Configuration de base du projet
# ============================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Générateurs automatiques de Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Où chercher les .ui
set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_SOURCE_DIR}/ui")

# ============================================================
# Trouver Qt
# ============================================================
find_package(Qt6 REQUIRED COMPONENTS Widgets)
# (Si Qt5 : remplace Qt6 par Qt5 partout)

# ============================================================
# Organisation des chemins
# ============================================================
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ============================================================
# Récupération des fichiers
# ============================================================
file(GLOB_RECURSE LO21_SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE LO21_HEADERS "${CMAKE_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE LO21_FORMS   "${CMAKE_SOURCE_DIR}/ui/*.ui")

# ============================================================
# Définition de l’exécutable
# ============================================================
add_executable(lo21
    ${LO21_SOURCES}
    ${LO21_HEADERS}
    ${LO21_FORMS}
    resources.qrc           # <-- ajoute ton fichier de ressources ici
)

# Inclure les headers du projet
target_include_directories(lo21 PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}   # ui_*, moc_*
)

# ============================================================
# Liaisons et options
# ============================================================
target_link_libraries(lo21 PRIVATE Qt6::Widgets)

if(MSVC)
    target_compile_options(lo21 PRIVATE /W4 /permissive-)
else()
    target_compile_options(lo21 PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ============================================================
# Messages utiles
# ============================================================
message(STATUS "Sources trouvées : ${LO21_SOURCES}")
message(STATUS "Headers trouvés : ${LO21_HEADERS}")
message(STATUS "UI trouvés : ${LO21_FORMS}")
