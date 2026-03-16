#include "Camera2D.h"

Camera2D::Camera2D() : x(0.0f), y(0.0f), zoom(32.0f),speed(50.0f) {}

Camera2D::~Camera2D() {}

void Camera2D::move(float deltaX, float deltaY) {
    x += deltaX;
    y += deltaY;
}

void Camera2D::setZoom(float newZoom) {
    zoom = clamp(newZoom, 4.0f, 4096.0f);
}

void Camera2D::zoomAt(float newZoom, int mouseX, int mouseY) {
    float oldZoom = zoom;
    
    setZoom(newZoom);
    if (zoom == oldZoom) return;

    // la coordonnée "Monde" pointée par la souris actuellement
    float worldX = (mouseX + x) / oldZoom;
    float worldY = (mouseY + y) / oldZoom;

    // On réajuste la caméra (x, y) pour que cette coordonnée "Monde" 
    // multipliée par le NOUVEAU zoom retombe exactement sur le (mouseX, mouseY)
    x = (worldX * zoom) - mouseX;
    y = (worldY * zoom) - mouseY;
}