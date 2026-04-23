#include "Application.h"
#include "ConfigManager.h"
#include <cstdlib>

int main() {
    srand(time(nullptr));
    ConfigManager::getInstance().loadConfig("config.json", "config/");
    int seed  = rand()%1000;
    Application app(1720,1080, 500, 500, 55);
    app.run();
    return 0;
}