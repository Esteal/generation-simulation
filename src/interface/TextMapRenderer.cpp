#include "TextMapRenderer.h"

char TextMapRenderer::biomeToChar(BiomeIndex biome)
{
    switch (biome) {
        case BiomeIndex::OCEAN:      return 'O';
        case BiomeIndex::PLAINS:     return 'P';
        case BiomeIndex::DESERT:     return 'D';
        case BiomeIndex::MOUNTAINS:  return 'M';
        case BiomeIndex::FOREST:     return 'F';
        case BiomeIndex::BEACH:      return 'B';
        case BiomeIndex::SNOW:       return 'S';
        case BiomeIndex::JUNGLE:     return 'J';
        case BiomeIndex::RIVER:      return 'R';
        case BiomeIndex::GLACE:      return 'G';
        default:                     return '?';
    }
}

void TextMapRenderer::printBiomeMap(const Map &map, size_t maxWidth, size_t maxHeight) const
{
    size_t w = map.getWidth();
    size_t h = map.getHeight();
    size_t displayW = std::min(w, maxWidth);
    size_t displayH = std::min(h, maxHeight);

    std::cout << "--- Affichage textuel des biomes (" << displayW << "x" << displayH << ") "
              << "sur " << w << "x" << h << " ---" << std::endl;

    for (size_t y = 0; y < displayH; ++y) {
        for (size_t x = 0; x < displayW; ++x) {
            const Cell &cell = map.getGrid().get((float)x, (float)y);
            std::cout << biomeToChar(cell.biome);
        }
        std::cout << '\n';
    }

    if (displayW < w || displayH < h) {
        std::cout << "carte redimensionnee pour affichage texte, pour voir l'entiereté de la carte => prendre des dimensions plus petites que " << w << "x" << h << ")\n";
    }
    std::cout << "-----------------------------------------" << std::endl;
}

bool TextMapRenderer::testRegression() {
    std::cout << "[Test] TextMapRenderer... ";
    
    // On vérifie que deux biomes distincts ne génèrent pas le même caractère
    char charOcean = TextMapRenderer::biomeToChar(BiomeIndex::OCEAN);
    char charPlains = TextMapRenderer::biomeToChar(BiomeIndex::PLAINS);
    
    // Un caractère nul '\0' indiquerait une erreur non gérée dans le switch de la méthode
    if (charOcean == '\0' || charPlains == '\0') {
        std::cerr << "Erreur: La conversion de biome a echoue (caractere nul)." << std::endl;
        return false;
    }
    
    if (charOcean == charPlains) {
        std::cerr << "Erreur: L'ocean et la plaine partagent le meme caractere." << std::endl;
        return false;
    }
    
    std::cout << "OK" << std::endl;
    return true;
}