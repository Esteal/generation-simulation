#include "TextureManager.h"
#include <iostream>

TextureManager::~TextureManager() 
{
    clean();
}

std::string TextureManager::findPath(const std::string& filePath) {
    char* basePath = SDL_GetBasePath();
    std::string imagePath = filePath;

    if (basePath) {
        // basePath contient le chemin vers l'exécutable (ex: ".../generationsimulation/bin/")
        // On concatène "../data/" pour pointer vers le bon dossier
        imagePath = std::string(basePath) + "../data/" + filePath;
        
        // libérer la mémoire allouée par SDL_GetBasePath
        SDL_free(basePath); 
    } else {
        std::cerr << "Erreur SDL_GetBasePath : " << SDL_GetError() << std::endl;
    }
    return imagePath;
}
bool TextureManager::load(const std::string& id, const std::string& filePath, Window& window)
{
    if (textures.find(id) != textures.end()) 
    {
        std::cout << "La texture '" << id << "' est deja chargee en memoire." << std::endl;
        return true; 
    }

    std::string fullPath = findPath(filePath);

    SDL_Texture* newTexture = window.loadTexture(fullPath);
    
    if (newTexture != nullptr) 
    {
        textures[id] = newTexture;
        std::cout << "Texture '" << id << "' chargee avec succes." << std::endl;
        return true;
    }
    
    return false;
}

void TextureManager::draw(const std::string& id, Window& window, int x, int y, int w, int h)
{
    auto it = textures.find(id);
    
    if (it != textures.end())
        window.drawTexture(it->second, x, y, w, h);
    else
        std::cout << "Erreur : Texture '" << id << "' non trouvée." << std::endl;
}

void TextureManager::drawFrame(const std::string& id, Window& window, int srcX, int srcY, int srcW, int srcH, int destX, int destY, int destW, int destH)
{
    auto it = textures.find(id);

    if (it != textures.end())
        window.drawTexture(it->second, srcX, srcY, srcW, srcH, destX, destY, destW, destH);
    else
        std::cout << "Erreur : Texture '" << id << "' non trouvée." << std::endl;
}

void TextureManager::clean()
{
    for (auto const& [id, texture] : textures)
        SDL_DestroyTexture(texture);
    
    textures.clear();
    //cout << "La texture map est clean()" << endl;
}

bool TextureManager::testRegression() {
    std::cout << "[Test] TextureManager... ";
    
    Window win("Test TM", 100, 100);
    if (!win.getIsInitialized()) {
        std::cerr << "\nErreur: Impossible d'initialiser la fenetre pour le test TM." << std::endl;
        return false;
    }

    TextureManager tm;
    
    std::string testPath = tm.findPath("fichier_test.png");
    if (testPath.empty()) {
        std::cerr << "\nErreur: findPath a retourne une chaine vide." << std::endl;
        return false;
    }
    if (testPath.find("fichier_test.png") == std::string::npos) {
        std::cerr << "\nErreur: findPath n'a pas inclus le nom du fichier d'origine dans le resultat." << std::endl;
        return false;
    }

    bool success = tm.load("BAD_ID", "faug.png", win);
    if (success) {
        std::cerr << "\nErreur: TextureManager a considere un faux fichier comme un succes." << std::endl;
        return false;
    }
    
    tm.draw("BAD_ID", win, 0, 0, 50, 50);
    tm.drawFrame("BAD_ID", win, 0, 0, 10, 10, 0, 0, 10, 10);
    tm.clean();
    
    std::cout << "OK" << std::endl;
    return true;
}