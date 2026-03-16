#ifndef WINDOW_H
#define WINDOW_H
#include "iostream"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
using namespace std;
class Window
{
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool isInitialized;
public:
    Window(const string &title, const int &width, const int &height);
    ~Window();
    SDL_Renderer& getRenderer();
    void clear();
    void present();
    SDL_Texture* loadTexture(const string& filePath);
    void drawTexture(SDL_Texture* texture, int x, int y, int w, int h);
    void drawTexture(SDL_Texture* texture, int srcX, int srcY, int srcW, int srcH, int destX, int destY, int destW, int destH);
};

#endif