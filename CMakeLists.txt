cmake_minimum_required(VERSION 3.10)
project(GenerationSimulation)

add_compile_options(-Wall -ggdb)
# 1. Rediriger les exécutables compilés vers le dossier "bin"
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# 2. Sources communes
set(SOURCE_FILES
    src/core/main.cpp
    src/core/Application.cpp
    src/core/MapGenerator.cpp
    src/core/system/HydrologieSystem.cpp
    src/interface/Window.cpp
    src/interface/TextureManager.cpp
    src/interface/Camera2D.cpp
    src/interface/MapRenderer.cpp)

# 3. Créer l'exécutable principal (compile main.cpp)
add_executable(main ${SOURCE_FILES})

# 4. Indiquer où trouver les fichiers .h (comme FastNoiseLite.h et Grid.h)
target_include_directories(main PRIVATE "${CMAKE_SOURCE_DIR}/include/core" 
                                        "${CMAKE_SOURCE_DIR}/include/interface"
                                        "${CMAKE_SOURCE_DIR}/include/core/system"
                                        ${SDL2_LIBRARIES} 
                                        ${SDL2_IMAGE_LIBRARIES})

# 5. Lier la bibliothèque SDL2 à l'exécutable
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
target_link_libraries(main PRIVATE SDL2::SDL2 SDL2_image::SDL2_image)