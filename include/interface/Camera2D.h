#ifndef CAMERA2D_H
#define CAMERA2D_H
#include "Window.h"
#include <algorithm>

using namespace std;

class Camera2D {
private:
    float x, y;      // Le centre de la vue dans le monde
    float zoom;
    float speed;     
public:
    // Constructeur
    Camera2D();
    ~Camera2D();
    float getX() const { return x; }
    float getY() const { return y; }
    float getSpeed() const { return speed; }
    float getZoom() const { return zoom; }
    void move(float deltaX, float deltaY);
    void setZoom(float newZoom);
    void zoomAt(float newZoom, int mouseX, int mouseY);
};

#endif