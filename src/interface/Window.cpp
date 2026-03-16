#include "Window.h"

Window::Window(const string &title, const int &width, const int &height)
    : window(nullptr), renderer(nullptr), isInitialized(false)
{
// Initialisation de SDL
    cout << "SDL: init" << endl;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cout << "Erreur lors de l'initialisation de la SDL : " << SDL_GetError() << endl;
        SDL_Quit();
        exit(1);
    }

    // Initialisation de SDL_image pour les textures png
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        cout << "Erreur lors de l'initialisation de SDL_image : " << IMG_GetError() << endl;
        SDL_Quit();
        exit(1);
    }

    // Creation de la fenetre
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == NULL)
    {
        cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << endl;
        SDL_Quit();
        exit(1);
    }

    // Création du renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        cout << "Erreur lors de la creation du renderer : " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }
    isInitialized = true;   
}

Window::~Window() 
{ 
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

void Window::clear()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Window::present() 
{
    SDL_RenderPresent(renderer);
}

SDL_Renderer& Window::getRenderer() 
{
    return *renderer;
}

SDL_Texture* Window::loadTexture(const string& filePath)
{
    // créer la Surface en RAM, puis l'envoyer en Texture dans la VRAM.
    
    SDL_Texture* texture = IMG_LoadTexture(renderer, filePath.c_str());
    
    if (texture == nullptr)
        cout << "Erreur de chargement de l'image " << filePath << " : " << IMG_GetError() << endl;
    
    return texture;
}

void Window::drawTexture(SDL_Texture* texture, int srcX, int srcY, int srcW, int srcH, int destX, int destY, int destW, int destH)
{
    if (texture == nullptr) return;

    // Le rectangle qui "découpe" la portion de la spritesheet
    SDL_Rect srcRect;
    srcRect.x = srcX;
    srcRect.y = srcY;
    srcRect.w = srcW;
    srcRect.h = srcH;

    // Le rectangle où l'image sera affichée sur la fenêtre
    SDL_Rect destRect;
    destRect.x = destX;
    destRect.y = destY;
    destRect.w = destW;
    destRect.h = destH;

    // On passe l'adresse de srcRect à SDL_RenderCopy
    SDL_RenderCopy(renderer, texture, &srcRect, &destRect);
}

void Window::drawTexture(SDL_Texture* texture, int x, int y, int w, int h)
{
    if (texture == nullptr) return;

    SDL_Rect destRect;
    destRect.x = x;
    destRect.y = y;
    destRect.w = w;
    destRect.h = h;

    SDL_RenderCopy(renderer, texture, nullptr, &destRect);
}