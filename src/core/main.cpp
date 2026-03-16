#include "Application.h"
#include <cstdlib>

int main() {
    srand(time(nullptr));
    int seed  = rand()%1000;
    Application app(1720,1080, 3000, 3000,seed);
    app.run();
    return 0;
}