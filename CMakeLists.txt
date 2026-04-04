cmake_minimum_required(VERSION 3.10)
project(GenerationSimulation)

add_compile_options(-Wall -ggdb)

# 1. Rediriger les exécutables compilés vers le dossier "bin"
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# 2. Sources communes (SANS main.cpp ni test_main.cpp)
set(COMMON_SOURCES
    src/core/Application.cpp
    src/core/Map.cpp
    src/core/MapGenerator.cpp
    src/core/system/HydrologieSystem.cpp
    src/core/system/WorldSimulator.cpp
    src/core/system/HydrolicErosionSystem.cpp
    src/core/system/ThermalErosionSystem.cpp
    src/core/system/VegetationSystem.cpp
    src/core/system/ISimulationSystem.cpp
    src/core/system/LightSystem.cpp
    src/core/system/CivilisationSystem.cpp
    src/interface/Window.cpp
    src/interface/TextureManager.cpp
    src/interface/Camera2D.cpp
    src/interface/MapRenderer2DIso.cpp
    src/interface/MapRenderer.cpp
    src/interface/TextMapRenderer.cpp
    src/interface/KeyboardHandler.cpp
)

# ==========================================
# CIBLE 1 : L'application principale
# ==========================================
add_executable(main src/main.cpp ${COMMON_SOURCES})

target_include_directories(main PRIVATE 
    "${CMAKE_SOURCE_DIR}/include/core" 
    "${CMAKE_SOURCE_DIR}/include/interface"
    "${CMAKE_SOURCE_DIR}/include/core/system"
)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
target_link_libraries(main PRIVATE SDL2::SDL2 SDL2_image::SDL2_image)


# ==========================================
# CIBLE 2 : L'exécutable de tests
# ==========================================
# On compile les mêmes sources communes, mais avec test_main.cpp
add_executable(run_tests src/testRegression.cpp ${COMMON_SOURCES})

# Inclusion des mêmes dossiers (header)
target_include_directories(run_tests PRIVATE 
    "${CMAKE_SOURCE_DIR}/include/core" 
    "${CMAKE_SOURCE_DIR}/include/interface"
    "${CMAKE_SOURCE_DIR}/include/core/system"
)

# On ajoute le flag "DEBUG_TESTS" UNIQUEMENT pour cet exécutable
target_compile_definitions(run_tests PRIVATE DEBUG_TESTS)

# On lie la SDL2 aux tests (utile si Window ou TextureManager sont testés)
target_link_libraries(run_tests PRIVATE SDL2::SDL2 SDL2_image::SDL2_image)


# ==========================================
# PARTIE DOXYGEN
# ==========================================
find_package(Doxygen)

if(DOXYGEN_FOUND)
    add_custom_target(docu
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/doc/doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Génération de la documentation avec Doxygen..."
        VERBATIM
    )
    message(STATUS "Doxygen trouvé")
else()
    message(WARNING "Doxygen non trouvé")
endif()