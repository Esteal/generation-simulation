#include "TextureManager.h"

TextureManager::~TextureManager() 
{
    clean();
}

bool TextureManager::load(const string& id, const string& filePath, Window& window)
{
    if (textures.find(id) != textures.end()) 
    {
        cout << "La texture '" << id << "' est deja chargee en memoire." << endl;
        return true; 
    }

    SDL_Texture* newTexture = window.loadTexture(filePath);
    
    if (newTexture != nullptr) 
    {
        textures[id] = newTexture;
        cout << "Texture '" << id << "' chargeg avec succes." << endl;
        return true;
    }
    
    return false;
}

void TextureManager::draw(const string& id, Window& window, int x, int y, int w, int h)
{
    auto it = textures.find(id);
    
    if (it != textures.end())
        window.drawTexture(it->second, x, y, w, h);
    else
        cout << "Erreur : Texture '" << id << "' non trouvée." << endl;
}

void TextureManager::drawFrame(const string& id, Window& window, int srcX, int srcY, int srcW, int srcH, int destX, int destY, int destW, int destH)
{
    auto it = textures.find(id);

    if (it != textures.end())
        window.drawTexture(it->second, srcX, srcY, srcW, srcH, destX, destY, destW, destH);
    else
        cout << "Erreur : Texture '" << id << "' non trouvée." << endl;
}

void TextureManager::clean()
{
    for (auto const& [id, texture] : textures)
        SDL_DestroyTexture(texture);
    
    textures.clear();
    cout << "La texture map est clean()" << endl;
}