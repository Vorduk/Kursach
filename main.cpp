#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "RayCastLib.h"
#include <chrono>
#include <windows.h>
#include <conio.h>
#include <cctype>

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
    }

    system("cls");
    saveMap("MAP2.txt", map);
}

int main()
{
    //Map my_map = loadMapFromFile("MAP1.txt");

    //Screen* my_screen = new Screen();

    //Camera* my_camera = new Camera();

    //mainLoop(*my_screen, my_map, *my_camera);

    //delete my_screen;
    //delete my_camera;

    
    Map map;

    // Добавим несколько врагов
    map.addEnemy(std::make_shared<Goblin>(50, 5, 5));
    map.addEnemy(std::make_shared<Orc>(30, 10, 10));
    map.addEnemy(std::make_shared<Dragon>(70, 15, 15));

    std::cout << "Enemies before sorting:" << std::endl;
    map.printEnemies();

    // Сортировк по здоровью
    map.sortEnemiesByHealth();
    std::cout << "Enemies after sorting by health:" << std::endl;
    map.printEnemies();

    // Поиск с опр. здоровьем
    int searchHealth = 30;
    auto foundEnemy = map.findEnemyByHealth(searchHealth);
    if (foundEnemy) {
        std::cout << "Found enemy with health " << searchHealth << std::endl;
    }
    else {
        std::cout << "No enemy found with health " << searchHealth << std::endl;
    }

    return 0;

    return 0;
}
