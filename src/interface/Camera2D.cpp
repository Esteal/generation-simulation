#include "Camera2D.h"
#include <iostream>
#include <algorithm>

Camera2D::Camera2D() : x(0.0f), y(0.0f), zoom(32.0f),speed(50.0f) {}

Camera2D::~Camera2D() {}

void Camera2D::move(float deltaX, float deltaY) {
    x += deltaX;
    y += deltaY;
}

void Camera2D::setPosition(float newX, float newY) {
    x = newX;
    y = newY;
}

void Camera2D::setZoom(float newZoom) {
    zoom = std::clamp(newZoom, 5.0f, 100.0f);
    //std::cout << zoom << std::endl; 
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

bool Camera2D::testRegression() {
    std::cout << "[Test] Camera2D... ";
    bool allPassed = true;

    // Fonction lambda utilitaire pour comparer des flottants en interne
    auto isClose = [](float a, float b) { return std::abs(a - b) < 0.001f; };

    // Instance dédiée au test
    Camera2D cam;

    // Test 1 : Valeurs par défaut du constructeur
    // Remplacez 0.0f et 1.0f par vos vraies valeurs d'initialisation
    if (!isClose(cam.x, 0.0f) || !isClose(cam.y, 0.0f) || !isClose(cam.zoom, 32.0f)|| !isClose(cam.speed, 50.0f)) {
        std::cerr << "[ECHEC] Constructeur : valeurs initiales incorrectes." << std::endl;
        allPassed = false;
    }

    // Test 2 : Méthode move()
    cam.move(15.5f, -4.2f);
    if (!isClose(cam.x, 15.5f) || !isClose(cam.y, -4.2f)) {
        std::cerr << "[ECHEC] move() : la position n'a pas ete mise a jour correctement." << std::endl;
        allPassed = false;
    }

    // Test 3 : Méthode setZoom()
    cam.setZoom(5.0f);
    if (!isClose(cam.zoom, 5.0f)) {
        std::cerr << "[ECHEC] setZoom() : le zoom n'a pas ete applique." << std::endl;
        allPassed = false;
    }

    // Conclusion
    if (allPassed) {
        std::cout << "OK" << std::endl;
    } else {
        std::cerr << "[ERREUR] Camera2D presente des regressions." << std::endl;
    }

    return allPassed;
}
