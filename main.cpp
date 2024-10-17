#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "RayCastLib.h"
#include <chrono>
#include <windows.h>
#include <conio.h>

// Gets screen width
int getWidthOfScreen(Screen screen) {
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
    double step_f = 0.2; // Speed of foward movement of player
    double step_b = -0.2; // Speed of backward movement of player
    double step_s = 0.15; // Speed of left/right movement of player
    double ray_step = 0.003; // Speed of camera rotation

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);

    int cc = 1;

    int x, y;
    int prev_x = 0;

    int exit = 0;
    double fps = 0;
    while (!exit) {
        map.getPlayer().setPlayerPrevX();
        map.getPlayer().setPlayerPrevY();

        auto start = std::chrono::high_resolution_clock::now();

        if (map.getPlayer().getPlayerAngle() >= 180) {
            map.getPlayer().setPlayerAngle(0);;
        }
        if (map.getPlayer().getPlayerAngle() < 0) {
            map.getPlayer().setPlayerAngle(179.99);
        }

        screen.clearScreen();
        camera.synchronizeWithPlayer(map.getPlayer());
        renderDDA(map, camera, screen);
        //render(map, camera, screen); // Old render (slower)
        drawMap(map, camera, screen);
        screen.drawText("FPS: " + std::to_string(static_cast<int>(fps)), 0, 200);
        screen.drawText("Player x: " + std::to_string(map.getPlayer().getPlayerX()), 1, 200);
        screen.drawText("Player y: " + std::to_string(map.getPlayer().getPlayerY()), 2, 200);
        screen.drawText("Player hp: " + std::to_string(map.getPlayer().getPlayerHealth()), 3, 200);
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

            int key = _getch();

            switch (key) {
            case 'w':
                map.getPlayer().movePlayer(step_f);
                break;
            case 's':
                map.getPlayer().movePlayer(step_b);
                break;
            case 'a':
                map.getPlayer().movePlayerSide(step_s);
                break;
            case 'd':
                map.getPlayer().movePlayerSide(-step_s);
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

        processPlayerCollision(map);

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
                map.getPlayer().incrementPlayerAngle(ray_step * abs(x - prev_x));
            }
            else if (x < prev_x) { // Mouse moved left
                map.getPlayer().decrementPlayerAngle(ray_step * abs(x - prev_x));
            }
            prev_x = x; // Update previous x coordinate
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        fps = 1.0 / elapsed.count();

        //std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    system("cls");
    saveMap("MAP2.txt", map);
}

void mainLoop2(Screen screen1, Map map1, Camera camera1, Screen screen2, Map map2, Camera camera2) {
    processPlayerCollision(map1);
    processPlayerCollision(map2);

    double step_f = 0.2; // Speed of foward movement of player
    double step_b = -0.2; // Speed of backward movement of player
    double step_s = 0.15; // Speed of left/right movement of player
    double ray_step = 0.003; // Speed of camera rotation

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);

    int cc = 1;

    int x, y;
    int prev_x = 0;

    int exit = 0;
    double fps = 0;
    while (!exit) {

        auto start = std::chrono::high_resolution_clock::now();

        if (map1.getPlayer().getPlayerAngle() >= 180) {
            map1.getPlayer().setPlayerAngle(0);;
        }
        if (map1.getPlayer().getPlayerAngle() < 0) {
            map1.getPlayer().setPlayerAngle(179.99);
        }

        if (map2.getPlayer().getPlayerAngle() >= 180) {
            map2.getPlayer().setPlayerAngle(0);
        }
        if (map2.getPlayer().getPlayerAngle() < 0) {
            map2.getPlayer().setPlayerAngle(179.99);
        }

        screen1.clearScreen();
        screen2.clearScreen();

        camera1.synchronizeWithPlayer(map1.getPlayer());
        camera2.synchronizeWithPlayer(map2.getPlayer());

        renderDDA(map1, camera1, screen1);
        renderDDA(map2, camera2, screen2);

        drawMap(map1, camera1, screen1);
        drawMap(map2, camera2, screen2);

        //render(map, camera, screen); // Old render (slower)
        screen1.drawText("FPS: " + std::to_string(static_cast<int>(fps)), 0, 20);
        screen2.drawText("FPS: " + std::to_string(static_cast<int>(fps)), 0, 20);

        screen1.printScreen();
        screen2.printScreen();

        screen1.setToBeginning();
        screen2.setToBeginning();

        if (cc == -1) {
            CONSOLE_CURSOR_INFO info;
            info.dwSize = 100;
            info.bVisible = false;
            SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
        }

        // Keyboard player movement
        if (_kbhit()) {

            int key = _getch();

            switch (key) {
            case 'w':
                map1.getPlayer().movePlayer(step_f);
                map2.getPlayer().movePlayer(step_f);
                break;
            case 's':
                map1.getPlayer().movePlayer(step_b);
                map2.getPlayer().movePlayer(step_b);
                break;
            case 'a':
                map1.getPlayer().movePlayerSide(step_s);
                map2.getPlayer().movePlayerSide(step_s);
                break;
            case 'd':
                map1.getPlayer().movePlayerSide(-step_s);
                map2.getPlayer().movePlayerSide(-step_s);
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

            int w_cons = getWidthOfScreen(screen1);
            if (x >= w_cons - 5) { // Cursor hits right edge with a 5-pixel threshold
                SetCursorPos(w_cons - 1, y); // Clamp to right edge
                prev_x = w_cons - 1;
            }
            else if (x <= 60) { // Cursor hits left edge with a 20-pixel threshold
                if (x <= prev_x) { // Cursor is moving towards the left edge
                    SetCursorPos(60, y); // Clamp to left edge with a small offset
                    prev_x = 60;
                }
                else {
                    SetCursorPos(x, y); // Set cursor to current position
                    prev_x = x;
                }
            }

            if (x > prev_x) { // Mouse moved right
                map1.getPlayer().incrementPlayerAngle(ray_step * abs(x - prev_x));
                map2.getPlayer().incrementPlayerAngle(ray_step * abs(x - prev_x));
            }
            else if (x < prev_x) { // Mouse moved left
                map1.getPlayer().decrementPlayerAngle(ray_step * abs(x - prev_x));
                map2.getPlayer().decrementPlayerAngle(ray_step * abs(x - prev_x));
            }
            prev_x = x; // Update previous x coordinate
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        fps = 1.0 / elapsed.count();

        //std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    screen1.clearScreen();
}

int main()
{
    Map my_map = loadMapFromFile("MAP1.txt");
    Map my_map2 = loadMapFromFile("MAP2.txt");

    Screen* my_screen = new Screen(30, 115);
    Screen* my_screen2 = new Screen(30, 115);
    Screen* my_screen3 = new Screen();

    Camera* my_camera = new Camera();
    Camera* my_camera2 = new Camera();

    //mainLoop(*my_screen, my_map, *my_camera);
    //mainLoop2(*my_screen, my_map, *my_camera, *my_screen2, my_map2, *my_camera2);

    mainLoop(*my_screen3, my_map, *my_camera);

    delete my_screen;
    delete my_screen2;
    delete my_screen3;

    delete my_camera;
    delete my_camera2;

    return 0;
}
