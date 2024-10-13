#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "RayCastLib.h"
#include <chrono>
#include <windows.h>
#include <conio.h>

// Gets screen width
int getWidthOfScreen(struct Screen screen) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);

    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    GetCurrentConsoleFontEx(hConsole, FALSE, &cfi);

    int width_in_pixels = screen.getScrWidth() * cfi.dwFontSize.X;

    return width_in_pixels;
}


void mainLoop(Screen screen, Map map, Camera camera) {
    Player player = map.getPlayer();

    double step_f = 0.2; // Speed of foward movement of player
    double step_b = -0.1; // Speed of backward movement of player
    double step_s = 0.15; // Speed of left/right movement of player
    double ray_step = 0.003; // Speed of camera rotation

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);

    int cc = 1;

    int x, y;
    int prev_x = 0;

    int exit = 0;

    while (!exit) {
        if (player.getPlayerAngle() >= 180) {
            player.setPlayerAngle(0);;
        }
        if (player.getPlayerAngle() < 0) {
            player.setPlayerAngle(179.99);
        }

        screen.clearScreen();
        camera.synchronizeWithPlayer(player);
        renderDDA(map, camera, screen);
        screen.printScreen();
        screen.setToBeginning();

        if (cc == -1) {
            CONSOLE_CURSOR_INFO info;
            info.dwSize = 100;
            info.bVisible = false;
            SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
        }

        // Keyboard player movement
        if (_kbhit()) {

            int key = _getche();

            switch (key) {
            case 'w':
                player.movePlayer(step_f);
                break;
            case 's':
                player.movePlayer(step_b);
                break;
            case 'a':
                player.movePlayerSide(step_s);
                break;
            case 'd':
                player.movePlayerSide(-step_s);
                break;
            case 'c':
                cc = cc * -1;
                break;
            case 27:
                exit = 1;
                break;
            default: break;
            }
        }

        if (cc == -1) {
            POINT cursor_pos;
            GetCursorPos(&cursor_pos);
            x = cursor_pos.x;
            y = cursor_pos.y;

            int w_cons = getWidthOfScreen(screen);
            if (x >= w_cons - 5) { // Cursor hits right edge with a 5-pixel threshold
                SetCursorPos(w_cons - 1, y); // Clamp to right edge
                prev_x = w_cons - 1;
            }
            else if (x <= 20) { // Cursor hits left edge with a 20-pixel threshold
                if (x <= prev_x) { // Cursor is moving towards the left edge
                    SetCursorPos(20, y); // Clamp to left edge with a small offset
                    prev_x = 20;
                }
                else {
                    SetCursorPos(x, y); // Set cursor to current position
                    prev_x = x;
                }
            }

            if (x > prev_x) { // Mouse moved right
                player.incrementPlayerAngle(ray_step * abs(x - prev_x));
            }
            else if (x < prev_x) { // Mouse moved left
                player.decrementPlayerAngle(ray_step * abs(x - prev_x));
            }
            prev_x = x; // Update previous x coordinate
        }
    }

    screen.clearScreen();
    saveMap("MAP2.txt", map);
}

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

    float a = 0;
    //float deltaTime = 0.0f; // время между кадрами
    //auto lastTime = std::chrono::high_resolution_clock::now();

    Screen my_screen = Screen();

    Camera my_camera = Camera();

    mainLoop(my_screen, my_map, my_camera);

}
