#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <corecrt_math_defines.h>
#include <cstdio>
#include <cmath>

//Coords: (screen, map, player)
// +-------> x
// | 0000000
// | 0000000
// v 0000000
// y

#define DEFAULT_PLAYER_Y 0
#define DEFAULT_PLAYER_X 0
#define DEFAULT_PLAYER_ANGLE 0

#define DEFAULT_CAMERA_Y 0
#define DEFAULT_CAMERA_X 0
#define DEFAULT_CAMERA_ANGLE 0
#define DEFAULT_CAMERA_FOV_C 50
#define DEFAULT_RENDER_DISTANCE 1000

#define DEFAULT_MAP_HEIGHT 20
#define DEFAULT_MAP_WIDTH 20

#define DEFAULT_SCR_HEIGHT 60
#define DEFAULT_SCR_WIDTH 230

#define MINI_MAP_WIDTH 17
#define MINI_MAP_HEIGHT 9

// Player
class Player {
private:
    double m_player_y;
    double m_player_x;
    double m_player_angle;
public:
    // Default constructor
    Player() {
        m_player_y = DEFAULT_PLAYER_Y;
        m_player_x = DEFAULT_PLAYER_X;
        m_player_angle = DEFAULT_PLAYER_ANGLE;
    };
    //
    // Constructor
    Player(double player_y, double player_x, double player_angle) {
        m_player_y = player_y;
        m_player_x = player_x;
        m_player_angle = player_angle;
    };
    //
    // Destructor
    ~Player() {

    };
    // 
    // Setters and getters
    void setPlayerCoords(double player_y, double player_x) {
        m_player_y = player_y;
        m_player_x = player_x;
    }
    void setPlayerY(double player_y) {
        m_player_y = player_y;
    }
    double getPlayerY() {
        return m_player_y;
    }
    void setPlayerX(double player_x) {
        m_player_x = player_x;
    }
    double getPlayerX() {
        return m_player_x;
    }
    void setPlayerAngle(double player_angle) {
        m_player_angle = player_angle;
    }
    double getPlayerAngle() {
        return m_player_angle;
    }
    //
    // Move player foward or backward
    void movePlayer(double step) {
        m_player_x = m_player_x + step * cos(m_player_angle);
        m_player_y = m_player_y + step * sin(m_player_angle);
    }
    //
    // Move player right or left
    void movePlayerSide(double step) {
        m_player_x = m_player_x + step * sin(m_player_angle);
        m_player_y = m_player_y - step * cos(m_player_angle);
    }
    //
    // Move player diagonally
    void movePlayerDiagonal(double step_forward, double step_side) {
        m_player_x = m_player_x + step_forward * cos(m_player_angle) + step_side * sin(m_player_angle);
        m_player_y = m_player_y + step_forward * sin(m_player_angle) - step_side * cos(m_player_angle);
    }
    //
    // Add to angle
    void incrementPlayerAngle(double value) {
        m_player_angle += value;
    }
    //
    // Substract from angle
    void decrementPlayerAngle(double value) {
        m_player_angle -= value;
    }
};

// Camera
class Camera {
private:
    double m_camera_y;
    double m_camera_x;
    double m_camera_angle;
    double m_fov_c;
    double m_fov;
    double m_render_distance;
public:
    // Default constructor
    Camera() {
        m_camera_y = DEFAULT_CAMERA_Y;
        m_camera_x = DEFAULT_CAMERA_X;
        m_camera_angle = DEFAULT_CAMERA_ANGLE;
        m_fov_c = DEFAULT_CAMERA_FOV_C;
        m_fov = (m_fov_c / 180 * M_PI);
        m_render_distance = DEFAULT_RENDER_DISTANCE;
    }
    //
    // Constructor
    Camera(double camera_y, double camera_x, double camera_angle, double fov_c, double render_distance) {
        m_camera_y = camera_y;
        m_camera_x = camera_x;
        m_camera_angle = camera_angle;
        m_fov_c = fov_c;
        m_fov = (fov_c / 180 * M_PI);
        m_render_distance = render_distance;
    }
    //
    // Destructor
    ~Camera() {

    };
    // 
    // Setters and getters
    void setCameraCoords(double camera_y, double camera_x) {
        m_camera_y = camera_y;
        m_camera_x = camera_x;
    }
    void setCameraY(double camera_y) {
        m_camera_y = camera_y;
    }
    double getCameraY() {
        return m_camera_y;
    }
    void setCameraX(double camera_x) {
        m_camera_x = camera_x;
    }
    double getCameraX() {
        return m_camera_x;
    }
    void setCameraAngle(double camera_angle) {
        m_camera_angle = camera_angle;
    }
    double getCameraAngle() {
        return m_camera_angle;
    }
    void setFovC(double fov_c) {
        m_fov_c = fov_c;
    }
    double getFovC() {
        return m_fov_c;
    }
    void setFov(double fov) {
        m_fov = fov;
    }
    double getFov() {
        return m_fov;
    }
    void setRenderDistance(double render_distance) {
        m_render_distance = render_distance;
    }
    double getRenderDistance() {
        return m_render_distance;
    }
    //
    // Synchronizes camera with player.
    // The camera is not synchronized automatically to be able to control it separately
    void synchronizeWithPlayer(Player& player) {
        setCameraCoords(player.getPlayerY(), player.getPlayerX());
        m_camera_angle = player.getPlayerAngle();
    }
};

// Map
class Map {
private: 
	// Map dimensions
	int m_map_height;
	int m_map_width;
	std::vector<std::vector<char>> obstacle;
    Player m_player;

public: 
	// Default constructor
	Map() : m_map_height(DEFAULT_MAP_HEIGHT), m_map_width(DEFAULT_MAP_WIDTH) {
		obstacle.resize(m_map_height, std::vector<char>(m_map_width, ' '));
        m_player = Player();
	}

	// Constructor
	Map(int map_height, int map_width, Player player) {
		m_map_height = map_height;
		m_map_width = map_width;
		obstacle.resize(m_map_height, std::vector<char>(m_map_width, ' '));
        m_player = player;
	};

	// Destructor
	~Map() {

	};

    int getMapHeight() const {
        return m_map_height;
    }

    int getMapWidth() const {
        return m_map_width;
    }

    // Set obstacle on pos (y, x)
    void setObstacle(int y, int x, char obstacleType) {
        if (y >= 0 && y < m_map_height && x >= 0 && x < m_map_width) {
            obstacle[y][x] = obstacleType;
        }
    }

    // Set obstacle on pos (y, x)
    char getObstacle(int y, int x) const {
        if (y >= 0 && y < m_map_height && x >= 0 && x < m_map_width) {
            return obstacle[y][x];
        }
        return ' ';
    }

    Player& getPlayer() {
        return m_player;
    }

    Player getPlayer() const {
        return m_player;
    }
};

// Screen
class Screen {
private:
    std::vector<std::vector<char>> m_screen;
    int m_scr_height;
    int m_scr_width;
public:
    // Default constructor
    Screen() : m_scr_height(DEFAULT_SCR_HEIGHT), m_scr_width(DEFAULT_SCR_WIDTH) {
        m_screen.resize(m_scr_height, std::vector<char>(m_scr_width, ' '));
    };
    //
    // Constructor
    Screen(int scr_height, int scr_width) : m_scr_height(scr_height), m_scr_width(scr_width) {
        m_screen.resize(m_scr_height, std::vector<char>(m_scr_width, ' '));
    };
    //
    // Destructor
    ~Screen() {

    };
    //
    // Setters and getters 
    void setScrRes(int scr_height, int scr_width) {
        m_scr_height = scr_height;
        m_scr_width = scr_width;
    }
    int getScrHeight() {
        return m_scr_height;
    }
    int getScrWidth() {
        return m_scr_width;
    }
    //
    // Clears screen
    void clearScreen() {
        for (auto& row : m_screen) {
            std::memset(row.data(), ' ', row.size());
        }
    }
    //
    // Outputs screen contents to the console
    void printScreen() {
        for (int i = 0; i < m_scr_height; i++) {
            fwrite(&m_screen[i][0], 1, m_scr_width, stdout);
            putchar('\n');
        }
    }
    //
    // Outputs screen contents to the console by coords
    void printScreenC(int x1, int y1, int x2, int y2) {
        // Create a new array to store the shifted elements
        std::vector<std::vector<char>> new_screen(m_scr_height, std::vector<char>(m_scr_width, ' '));

        // Copy the elements from the original array to the new array, shifting them by x1 and y1
        for (int i = 0; i < m_scr_height; i++) {
            for (int j = 0; j < m_scr_width; j++) {
                if (i >= y1 && i <= y2 && j >= x1 && j <= x2) {
                    new_screen[i][j] = m_screen[i-y1][j-x1];
                }
                else {
                    new_screen[i][j] = ' ';
                }
            }
        }

        // Print the new array
        for (int i = 0; i < m_scr_height; i++) {
            fwrite(&new_screen[i][0], 1, m_scr_width, stdout);
            putchar('\n');
        }
    }
    //
    // Returns the print carriage to the beginning
    void setToBeginning() {
        printf("\x1b[%dA", m_scr_height);
    }
    //
    // Returns the print carriage to the beginning
    void setToBeginningV(int val) {
        printf("\x1b[%dA", val);
    }
    //
    // Draw text
    void drawText(const std::string& str, int y, int x) {
        if (y < 0 || y >= m_scr_height || x < 0 || x >= m_scr_width) {
            std::cerr << "[error] drawText(): Starting position is out of screen bounds." << std::endl;
            return;
        }

        for (int i = 0; i < str.size(); ++i) {
            if (x + i >= m_scr_width) {
                break;
            }
            m_screen[y][x + i] = str[i];
        }
    }
    //
    // Draws dot on the screen buffer
    void drawDot(int y, int x, char color) {
        if (x < 0 || x >= m_scr_width) {
            std::cerr << "[error] drawDot: wrong x" << std::endl;
            return;
        }
        if (y < 0 || y >= m_scr_height) {
            std::cerr << "[error] drawDot: wrong y" << std::endl;
            return;
        }

        m_screen[y][x] = color;
    }
    //
    // Draws line with char color
    void drawLine(int x1, int y1, int x2, int y2, char color) {
        if (x1 < 0 || x1 >= m_scr_width) {
            std::cerr << "[error] drawLine: Wrong x1" << std::endl;
            return;
        }
        if (x2 < 0 || x2 >= m_scr_width) {
            std::cerr << "[error] drawLine: Wrong x2" << std::endl;
            return;
        }
        if (y1 < 0 || y1 >= m_scr_height) {
            std::cerr << "[error] drawLine: Wrong y1" << std::endl;
            return;
        }
        if (y2 < 0 || y2 >= m_scr_height) {
            std::cerr << "[error] drawLine: Wrong y2" << std::endl;
            return;
        }

        // Bresenham's algorithm

        int x = x1;
        int y = y1;

        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);

        int s1 = x1 < x2 ? 1 : -1;
        int s2 = y1 < y2 ? 1 : -1;

        int interchange = 0;

        if (dy > dx) {
            int temp = dx;
            dx = dy;
            dy = temp;
            interchange = 1;
        }

        int e = 2 * dy - dx;

        for (int i = 0; i <= dx; i++) {
            if (x >= 0 && x < m_scr_width && y >= 0 && y < m_scr_height) {
                m_screen[y][x] = color;
            }

            while (e >= 0) {
                if (interchange == 1)
                    x += s1;
                else
                    y += s2;

                e = e - 2 * dx;
            }

            if (interchange == 1)
                y += s2;
            else
                x += s1;

            e = e + 2 * dy;
        }
    }
    //
    // Draws rectangle
    void drawRect(int y1, int x1, int y2, int x2, char border_color, char fill_color) {
        if (x1 < 0 || x1 >= m_scr_width) {
            std::cerr << "[error] drawRect: Wrong x1" << std::endl;
            return;
        }
        if (x2 < 0 || x2 >= m_scr_width) {
            std::cerr << "[error] drawRect: Wrong x2" << std::endl;
            return;
        }
        if (y1 < 0 || y1 >= m_scr_height) {
            std::cerr << "[error] drawRect: Wrong y1" << std::endl;
            return;
        }
        if (y2 < 0 || y2 >= m_scr_height) {
            std::cerr << "[error] drawRect: Wrong y2" << std::endl;
            return;
        }

        int x = x2 > x1 ? x1 : x2;
        int x_fin = x + abs(x2 - x1);

        int y = y2 > y1 ? y1 : y2;
        int y_fin = y + abs(y2 - y1);

        for (int i = y; i < y_fin; i++) // y
        {
            for (int j = x; j < x_fin; j++) // x
            {
                m_screen[i][j] = fill_color;
                if ((i == y) || (i == y_fin - 1) || (j == x) || (j == x_fin - 1)) {
                    m_screen[i][j] = border_color;
                }
            }
        }
    }
    //
    // Draws dot without check
    void fDrawDot(int y, int x, char color) {
        m_screen[y][x] = color;
    }
    //
    // Gets dot
    char fGetDot(int y, int x) {
        return (m_screen[y][x]);
    }
};

// Load map from file by path
Map loadMapFromFile(const char* filename) {
    std::ifstream file(filename);

    if (!file) {
        throw std::runtime_error("Failed to open map file");
    }

    // Coords of player from file
    double player_y_file;
    double player_x_file;
    double player_angle_file;

    // Read coords of the player
    file >> player_y_file >> player_x_file >> player_angle_file;

    if (!file) {
        throw std::runtime_error("Failed to read coords or angle of the player from file");
    }

    // New player with param from file
    Player player_file(player_y_file, player_x_file, player_angle_file);

    // Dimensions of the map from file
    int map_h_file;
    int map_w_file;

    // Read map dimensions
    file >> map_h_file >> map_w_file;

    if (!file) {
        throw std::runtime_error("Failed to read map dimensions from file");
    }

    // New map with param from file
    Map map_file(map_h_file, map_w_file, player_file);

    // Read map's obstacle
    for (int i = 0; i < map_h_file; i++) {
        for (int j = 0; j < map_w_file; j++) {
            char obstacle_type;
            file >> obstacle_type;
            if (!file) {
                throw std::runtime_error("Failed to read map data from file");
            }
            // Skip the comma
            char comma;
            file >> comma;
            map_file.setObstacle(i, j, obstacle_type);
        }
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    file.close();

    return map_file;
}

// Save map to file by path
void saveMap(const char* filename, const Map& map) {
    std::ofstream file(filename);

    if (!file) {
        throw std::runtime_error("Failed to open map file for writing");
    }

    // Write coords of the player
    file << map.getPlayer().getPlayerY() << "\n" << map.getPlayer().getPlayerX() << "\n" << map.getPlayer().getPlayerAngle() << std::endl;

    // Write map dimensions
    file << map.getMapHeight() << "\n" << map.getMapWidth() << std::endl;

    // Write map's obstacles
    for (int i = 0; i < map.getMapHeight(); i++) {
        for (int j = 0; j < map.getMapWidth(); j++) {
            char obstacle_type = map.getObstacle(i, j);
            file << obstacle_type << ",";
        }
        file << std::endl;
    }

    file.close();
}

// Render scene
void renderDDA(Map& map, Camera& camera, Screen& screen) {
    char gradient[9] = { '.',':',';','=','+','x','X','$','@' }; // gradient for walls

    int rays_n = screen.getScrWidth();
    int scr_h = screen.getScrHeight();

    double cam_angle = camera.getCameraAngle();
    double cam_fov = camera.getFov();
    double cam_x = camera.getCameraX();
    double cam_y = camera.getCameraY();
    double render_distance = camera.getRenderDistance();

    int gridSizeX = map.getMapWidth();
    int gridSizeY = map.getMapHeight();

    for (int column = 0; column < rays_n; column++) {
        int is_corner = 0;

        int map_check_x = cam_x;
        int map_check_y = cam_y;

        int hit = 0;

        double ray_angle = cam_angle - (cam_fov / 2) + ((column * cam_fov) / rays_n);

        float dx = cos(ray_angle);
        float dy = sin(ray_angle);

        double unit_step_size_x = sqrt(1 + (dy / dx) * (dy / dx));
        double unit_step_size_y = sqrt(1 + (dx / dy) * (dx / dy));

        double ray_length_x;
        double ray_length_y;

        double step_x;
        double step_y;

        if (dx < 0)
        {
            step_x = -1;
            ray_length_x = (cam_x - float(map_check_x)) * unit_step_size_x;
        }
        else {
            step_x = 1;
            ray_length_x = (float(map_check_x + 1) - cam_x) * unit_step_size_x;
        }

        if (dy < 0)
        {
            step_y = -1;
            ray_length_y = (cam_y - float(map_check_y)) * unit_step_size_y;
        }
        else {
            step_y = 1;
            ray_length_y = (float(map_check_y + 1) - cam_y) * unit_step_size_y;
        }

        double cur_distance = 0.0;
        while (cur_distance < render_distance) {

            // walk
            if (ray_length_x < ray_length_y) {
                map_check_x += step_x;
                cur_distance = ray_length_x;
                ray_length_x += unit_step_size_x;
            }
            else {
                map_check_y += step_y;
                cur_distance = ray_length_y;
                ray_length_y += unit_step_size_y;
            }

            double dot_x = cam_x + dx * cur_distance;
            double dot_y = cam_y + dy * cur_distance;

            if (map_check_x >= 0 && map_check_x < map.getMapWidth() && map_check_y >= 0 && map_check_y < map.getMapHeight()) {
                if (map.getObstacle(map_check_y, map_check_x) == '1') {
                    hit = 1;
                    if ((fabs(dot_x - round(dot_x)) < 0.05) && (fabs(dot_y - round(dot_y)) < 0.05)) {
                        is_corner = 1;
                    }
                    break;
                }
            }
        }

        if (hit == 1) {
            double n_c = cur_distance * cos(ray_angle - cam_angle); // fish eye fix

            int ceiling = (double)(scr_h / 2) - (scr_h / n_c);
            int floorr = scr_h - ceiling;

            char color; // color of wall
            int grad_size = sizeof(gradient);
            if (int(cur_distance) >= grad_size) {
                color = gradient[0];
            }
            else {
                int color_index = int((grad_size - cur_distance));
                color = gradient[color_index];
            }

            // Corners of the wall blocks
            if (is_corner == 1) {
                color = '|';
            }

            for (int i = 0; i < scr_h; i++) {
                if (i <= ceiling)
                {
                    screen.fDrawDot(i, column, ' ');
                }
                if (i > ceiling && i <= floorr) {
                    screen.fDrawDot(i, column, color);
                }
                if (i > floorr) {
                    screen.fDrawDot(i, column, '-');
                }
            }
        }
    }

}

// Render scene
void render(Map& map, Camera& camera, Screen& screen) {
    char gradient[9] = { '.',':',';','=','+','x','X','$','@' }; // gradient for walls

    int rays_n = screen.getScrWidth();
    int scr_h = screen.getScrHeight();

    double cam_angle = camera.getCameraAngle();
    double cam_fov = camera.getFov();
    double cam_x = camera.getCameraX();
    double cam_y = camera.getCameraY();
    double render_distance = camera.getRenderDistance();

    int gridSizeX = map.getMapWidth();
    int gridSizeY = map.getMapHeight();

    for (int column = 0; column < rays_n; column++) {

        int hit_h = 0;
        int hit_a = 0;

        // Column ray angle
        double ray_angle = cam_angle - (cam_fov / 2) + ((column * cam_fov) / rays_n);;

        double ray_x = cos(ray_angle);
        double ray_y = sin(ray_angle);

        // flags
        int hit = 0; // ray hits wall
        int is_corner = 0;

        double cur_distance = 0;
        for (cur_distance; cur_distance <= render_distance; cur_distance += 0.05) {
            double dot_x = cam_x + cur_distance * ray_x;
            double dot_y = cam_y + cur_distance * ray_y;

            int grid_x = (int)dot_x;
            int grid_y = (int)dot_y;


            // if ray hits wall
            if (map.getObstacle(grid_y, grid_x) == '1') {
                hit = 1;
                if ((fabs(dot_x - round(dot_x)) < 0.05) && (fabs(dot_y - round(dot_y)) < 0.05)) {
                    is_corner = 1;
                }
                break;
            }
        }

        if (hit == 1) {
            double n_c = cur_distance * cos(ray_angle - cam_angle); // fish eye fix

            int ceiling = (double)(scr_h / 2) - (scr_h / n_c);
            int floorr = scr_h - ceiling;

            char color; // color of wall
            int grad_size = sizeof(gradient);
            if (int(cur_distance) >= grad_size) {
                color = gradient[0];
            }
            else {
                int color_index = int((grad_size - cur_distance));
                color = gradient[color_index];
            }

            // Corners of the wall blocks
            if (is_corner == 1) {
                color = '|';
            }

            for (int i = 0; i < scr_h; i++) {
                if (i <= ceiling)
                {
                    screen.fDrawDot(i, column, ' ');
                }
                if (i > ceiling && i <= floorr) {
                    screen.fDrawDot(i, column, color);
                }
                if (i > floorr) {
                    screen.fDrawDot(i, column, ' ');
                }
            }
        }
   
    }
}

// Drawing map
void drawMap(Map& map, Camera& camera, Screen& screen) {
    char wall_ch = '#';
    char heal_ch = 'h';
    char empty_ch = ' ';
    char border_ch = '+';
    char ray_ch = '.';
    char player_ch = 'P';

    int map_height = map.getMapHeight();
    int map_width = map.getMapWidth();

    screen.drawRect(0, 0, MINI_MAP_HEIGHT + 2, MINI_MAP_WIDTH + 2, '+', ' ');

    int p_x = map.getPlayer().getPlayerX();
    int p_y = map.getPlayer().getPlayerY();

    // Calculate the offset to center the player
    int offset_x = (int)map.getPlayer().getPlayerX() - MINI_MAP_WIDTH / 2;
    int offset_y = (int)map.getPlayer().getPlayerY() - MINI_MAP_HEIGHT / 2;

    // Draw the obstacle map, clipped to MINI_MAP_WIDTHxMINI_MAP_HEIGHT area around the playåer
    for (int i = 0; i < map_height; i++) {
        for (int j = 0; j < map_width; j++) {
            int screen_x = j - offset_x + 1;
            int screen_y = i - offset_y + 1;
            if (screen_x >= 1 && screen_x < MINI_MAP_WIDTH + 1 && screen_y >= 1 && screen_y < MINI_MAP_HEIGHT + 1) {
                if (map.getObstacle(i, j) == '0') {
                    screen.fDrawDot(screen_y, screen_x, empty_ch);
                }
                else {
                    screen.fDrawDot(screen_y, screen_x, wall_ch); 
                }
            }
        }
    }

    // Draw the field of vi ew sector
    double fov_half = camera.getFov();
    double start_angle = map.getPlayer().getPlayerAngle() - fov_half;
    double end_angle = map.getPlayer().getPlayerAngle() + fov_half;
    int ray_length = 5;
    int num_rays = (int)(fov_half / 0.1) + 1; // calculate the number of rays needed to cover the sector
    for (int i = 0; i < num_rays; i++) {
        double angle = start_angle + i * (fov_half * 2) / num_rays;
        for (int j = 1; j <= ray_length; j++) {
            int ray_x = (int)(MINI_MAP_WIDTH / 2 + 1 + j * cos(angle));
            int ray_y = (int)(MINI_MAP_HEIGHT / 2 + 1 + j * sin(angle));
            if (ray_x >= 1 && ray_x < MINI_MAP_WIDTH + 1 && ray_y >= 1 && ray_y < MINI_MAP_HEIGHT + 1) {
                if (screen.fGetDot(ray_y, ray_x) == ' ') {
                    screen.fDrawDot(ray_y, ray_x, ray_ch); // ray character
                }
            }
        }
    }


    // Draw the player
    screen.fDrawDot(MINI_MAP_HEIGHT / 2 + 1, MINI_MAP_WIDTH / 2 + 1, player_ch);
}




