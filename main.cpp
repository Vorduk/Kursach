#include <iostream>
#include "RayCastLib.h"
#include <chrono>

int main()
{
    Map my_map = loadMapFromFile("MAP1.txt");

    int h = my_map.getMapHeight();
    int w = my_map.getMapWidth();
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            
            std::cout << my_map.getObstacle(y, x);
        }
        std::cout << "\n";
    }
    saveMap("MAP2.txt", my_map);

    int a = 0;
    float speed = 150.0f; // скорость изменения переменной a
    float deltaTime = 0.0f; // время между кадрами
    auto lastTime = std::chrono::high_resolution_clock::now();

    Screen my_screen = Screen();
    Screen my_screen2 = Screen();

    while (1) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count() / 1000.0f;
        lastTime = currentTime;

        a += speed * deltaTime;
        if (a > 50) {
            a = 0;
        }

        my_screen.clearScreen();
        my_screen.drawDot(20, (int)a, '@');
        my_screen.printScreen();
        my_screen.setToBeginning();

        my_screen2.clearScreen();
        my_screen2.drawRect(0, 0, 20, 40, '#', ' ');
        my_screen2.drawDot(30, (int)a, 'G');
        my_screen2.printScreenС(60, 10, 80, 50);
        my_screen.setToBeginning();
    }
    
}
