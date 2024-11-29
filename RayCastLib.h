#pragma once
#include <iostream>

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <corecrt_math_defines.h>
#include <cstdio>
#include <cmath>
#include <sstream>
#include <memory>
#include <limits>
#include <stdexcept>

//Coords: (screen, map, player)
// +-------> x
// | 0000000
// | 0000000
// v 0000000
// y

#define DEFAULT_PLAYER_HEALTH 100
#define DEFAULT_PLAYER_Y 0
#define DEFAULT_PLAYER_X 0
#define DEFAULT_PLAYER_ANGLE 0

#define LAVA_DAMAGE 1

#define DEFAULT_CAMERA_Y 0
#define DEFAULT_CAMERA_X 0
#define DEFAULT_CAMERA_ANGLE 0
#define DEFAULT_CAMERA_FOV_C 40
#define DEFAULT_RENDER_DISTANCE 1000

#define DEFAULT_MAP_HEIGHT 20
#define DEFAULT_MAP_WIDTH 20

#define DEFAULT_SCR_HEIGHT 60
#define DEFAULT_SCR_WIDTH 230

#define MINI_MAP_WIDTH 17
#define MINI_MAP_HEIGHT 9

#define FIRST_AID_HP 15

// Sprite
class Sprite {
private:
    int m_duration;
    int m_sprite_height;
    int m_sprite_width;
    std::vector<std::vector<std::vector<char>>> m_frames;
public:
    Sprite() {
        m_duration = 0;
        m_sprite_height = 0;
        m_sprite_width = 0;
    }
    // Construuctor
    Sprite(int duration, int height, int width)
        : m_duration(duration), m_sprite_height(height), m_sprite_width(width) {}
    //
    // Getters and setters
    int getDuration() const { return m_duration; }
    void setDuration(int duration) { m_duration = duration; }
    int getSpriteHeight() const { return m_sprite_height; }
    void setSpriteHeight(int height) { m_sprite_height = height; }
    int getSpriteWidth() const { return m_sprite_width; }
    void setSpriteWidth(int width) { m_sprite_width = width; }
    const std::vector<std::vector<char>>& getFrame(int frameIndex) const {
        if (frameIndex < 0 || frameIndex >= m_frames.size()) {
            std::cerr << "[ERROR]: Sprite.getFrame(): Frame index out of range" << std::endl;
        }
        return m_frames[frameIndex];
    }
    //
    // Adds new frame to sprite
    void addFrame(const std::vector<std::vector<char>>& frame) {
        if (frame.size() != m_sprite_height || frame[0].size() != m_sprite_width) {
            std::cerr << "[ERROR]: Sprite.AddFrame(): Frame dimensions do not match sprite dimensions." << std::endl;
        }
        m_frames.push_back(frame);
    }
    // Save sprite to .txt file by name
    void saveSpriteToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("[ERROR]: Sprite.saveSpriteToFile(): Unable to open file for writing.");
        }

        file << m_duration << '\n';
        file << m_sprite_height << '\n';
        file << m_sprite_width << '\n';

        for (size_t i = 0; i < m_frames.size(); ++i) {
            file << i << '\n'; // frame number between frames
            for (const auto& row : m_frames[i]) {
                for (char c : row) {
                    file << c;
                }
                file << '\n';
            }
        }
    }
    //
    // Load from file
    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("[ERROR]: Sprite.loadFromFile(): Unable to open file for reading.");
        }

        file >> m_duration;
        file >> m_sprite_height;
        file >> m_sprite_width;
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // skip 0 m_frames.clear();
        std::string line;

        int c = 1;
        while (std::getline(file, line)) {
            if (c % m_sprite_width != 0) {
                std::vector<std::vector<char>> frame;
                for (int i = 0; i < m_sprite_height; ++i) {
                    std::getline(file, line);
                    std::vector<char> row(line.begin(), line.end()); // создаем строку из символов
                    frame.push_back(row);
                }
                addFrame(frame);
            }
            else {
                continue;
            }
            c++;
        }
    }
    
    // Lab5
    // Можно получать доступ к кадру с помощью []
    const std::vector<std::vector<char>>& operator[](size_t index) const {
        return m_frames[index];
    }
    // Пергрузка + чтобы прибавлять кадры к спрайту
    Sprite operator+(const std::vector<std::vector<char>>& newFrame) {
        if (newFrame.size() != m_sprite_height || newFrame[0].size() != m_sprite_width) {
            std::cerr << "[ERROR]: Sprite.operator+: New frame dimensions do not match sprite dimensions." << std::endl;
        }

        // Создание нового спрайта с увеличенной длительностью
        Sprite newSprite = *this; // Копия текщего
        newSprite.m_duration += 1; // Увеличение длительности
        newSprite.addFrame(newFrame); // Добавление нового кадра

        return newSprite;
    }

};

// Enemy
class Enemy {
protected:
    int m_health;
    double m_x, m_y;
    int m_cur_sprite_frame;

    // Lab 5
    static int enemyCount;
public:

    // Constructor
    Enemy(int hp, double y, double x) : m_health(hp), m_x(x), m_y(y) {
        m_cur_sprite_frame = 0;

        // Lab 5
        enemyCount++;
    }

    //
    // Destructor
    virtual ~Enemy() 
    {
        // Lab 5
        enemyCount--;
    };
    //
    // Getters and setters
    // Lab 5
    static int getEnemyCount() {
        return enemyCount;
    }
    int getHealth() const {
        return m_health;
    }
    double getX() const {
        return m_x;
    }
    double getY() const {
        return m_y;
    }

    virtual void attack() {
        std::cout << "Enemy attacks" << std::endl;
    }
};
int Enemy::enemyCount = 0;
class Goblin : public Enemy {
public:
    Goblin(int hp, double x, double y) : Enemy(hp, x, y) {}

    void attack() override {
        std::cout << "Goblin attacks" << std::endl;
    }
};
class Orc : public Enemy {
public:
    Orc(int hp, double x, double y) : Enemy(hp, x, y) {}

    void attack() override {
        std::cout << "Orc attacks" << std::endl;
    }
};

// Item (items)
class Item {
protected:
    double m_item_x;
    double m_item_y;
    unsigned int m_amount;
    unsigned int m_max; // max in inventory cell
    Sprite m_cell_item_sprite;
    Sprite m_item_sprite;
public:
    // Constructors
    Item() : m_item_x(0.0), m_item_y(0.0), m_amount(1), m_max(64) {}
    Item(double x, double y, unsigned int amount, unsigned int max, const Sprite& cell_item_sprite, const Sprite& item_sprite) 
        : m_item_x(x), m_item_y(y), m_amount(amount), m_max(max), m_cell_item_sprite(cell_item_sprite), m_item_sprite(item_sprite) {
        setAmount(amount);
    }

    // Copy constructor
    Item(const Item& other)
        : m_item_x(other.m_item_x), m_item_y(other.m_item_y), m_amount(other.m_amount), m_max(other.m_max), m_cell_item_sprite(other.m_cell_item_sprite), m_item_sprite(other.m_item_sprite) {}

    // Virtual destructor
    virtual ~Item() {}

    // Getters and setters
    void setItemX(double x) {
        m_item_x = x;
    }
    double getItemX() const {
        return m_item_x;
    }
    void setItemY(double y) {
        m_item_y = y;
    }
    double getItemY() const {
        return m_item_y;
    }
    unsigned int getAmount() const {
        return m_amount;
    }
    void setAmount(int amount) {
        if (amount <= 0)
        {
            delete this;
        }
        else m_amount = amount;
    }
    unsigned int getMax() const {
        return m_max;
    }
    void setCellItemSprite(const Sprite& sprite)
    {
        m_cell_item_sprite = sprite;
    }
    Sprite getCellItemSprite() const {
        return m_cell_item_sprite;
    }
    void setItemSprite(const Sprite& sprite)
    {
        m_item_sprite = sprite;
    }
    Sprite getItemSprite() const {
        return m_item_sprite;
    }

    virtual std::string getType() const = 0;

    virtual bool isFinal() const { return false; }
};
// Gun
class Gun : public Item {
protected:
    int m_damage;
    int area;
public:
    // Constructor
    Gun(double x, double y, unsigned int amount, const Sprite& gun_cell_sprite, const Sprite& gun_sprite, int damage, int area)
        : Item(x, y, amount, 1, gun_cell_sprite, gun_sprite), m_damage(damage), area(area) {}

    // Virtual destructor
    virtual ~Gun() {}

    // Getters and setters
    void setDamage(int damage) {
        m_damage = damage;
    }
    int getDamage() const {
        return m_damage;
    }
    void setArea(int area_value) {
        area = area_value;
    }
    int getArea() const {
        return area;
    }

    std::string getType() const override {
        return "Gun";
    }

    virtual bool isFinal() const { return false; }
};
// Guns variants
class Pistol : public Gun {
public:
    Pistol(double x, double y, unsigned int amount, const Sprite& pistol_cell_sprite, const Sprite& pistol_sprite, int damage = 30, int area = 0)
        : Gun(x, y, amount, pistol_cell_sprite, pistol_sprite, damage, area) {}

    std::string getType() const override {
        return "Pistol";
    }
    virtual bool isFinal() const { return true; }
};
class Rifle : public Gun {
public:
    Rifle(double x, double y, unsigned int amount, const Sprite& riflel_cell_sprite, const Sprite& rifle_sprite, int damage = 50, int area = 0)
        : Gun(x, y, amount, riflel_cell_sprite, rifle_sprite, damage, area) {}

    std::string getType() const override {
        return "Rifle";
    }
    virtual bool isFinal() const { return true; }
};
// Armor
class Armor : public Item {
protected:
    int m_defense;

public:
    // Constructor
    Armor(double x, double y, unsigned int amount, const Sprite& armor_cell_sprite, const Sprite& armor_sprite, int defense)
        : Item(x, y, amount, 1, armor_cell_sprite, armor_sprite), m_defense(defense) {}

    // Virtual destructor
    virtual ~Armor() {}

    void setDefense(int defense) {
        m_defense = defense;
    }
    int getDefense() const {
        return m_defense;
    }

    std::string getType() const override {
        return "Armor";
    }
    virtual bool isFinal() const { return true; }
};
// First Aid Kit
class FirstAidKit : public Item {
protected:
    int m_heal;
public:
    FirstAidKit(double x, double y, unsigned int amount, const Sprite& first_aid_kit_cell_sprite, const Sprite& first_aid_kit_sprite, int heal)
        : Item(x, y, amount, 10, first_aid_kit_cell_sprite, first_aid_kit_sprite), m_heal(heal) {}

    int getHeal() const {
        return m_heal;
    }

    std::string getType() const override {
        return "FirstAidKit";
    }
    virtual bool isFinal() const { return true; }
};
class Ammo : public Item {
protected:
    int m_one_box_amount;
public:
    Ammo(double x, double y, unsigned int amount, const Sprite& ammo_cell_sprite, const Sprite& ammo_sprite, int one_box_amount)
        : Item(x, y, amount, 10, ammo_cell_sprite, ammo_sprite), m_one_box_amount(one_box_amount) {}

    int getOneBoxAmount() const {
        return m_one_box_amount;
    }

    std::string getType() const override {
        return "Ammo";
    }
    virtual bool isFinal() const { return true; }
};

// Inventory Cell
class InventoryCell {
private:
    Item* m_item; // Item ptr
    Sprite m_cell_sprite; // Cell sprite
    bool m_is_rendered; // is cell rendered
    Sprite m_cell_item_sprite; // Cell sprite
public:
    InventoryCell(const Sprite& sprite) : m_item(nullptr), m_cell_sprite(sprite), m_is_rendered(false)
    {

    }

    // Getters and setters
    void setItem(Item* item) {
        if (item->isFinal()) {
            m_cell_item_sprite = item->getCellItemSprite();
            m_item = item;
        }
    }
    Item* getItem() const {
        return m_item;
    }
    void setCellSprite(const Sprite& sprite) {
        m_cell_sprite = sprite;
    }
    Sprite getCellSprite() const {
        return m_cell_sprite;
    }
    void setCellItemSprite(const Sprite& sprite)
    {
        m_cell_item_sprite = sprite;
    }
    Sprite getCellItemSprite() const {
        return m_cell_item_sprite;
    }
    bool isRendered() const {
        return m_is_rendered;
    }
    void setRendered(bool rendered) {
        m_is_rendered = rendered;
    }
    void addItem(Item* added_item)
    {
        // if same item type or not final children
        if (!(added_item->getType() == m_item->getType()) || !added_item->isFinal()) {
            return;
        }

        // determine amount of both items
        if (m_item != nullptr) {
            int max = m_item->getMax();
            int current_amount = m_item->getAmount();
            int added_item_amount = added_item->getAmount();

            if (max > current_amount) {
                m_item->setAmount(current_amount + abs(added_item_amount-(max - current_amount)));
                added_item->setAmount(added_item_amount - abs(added_item_amount - (max - current_amount)));
            }
        }
        else {
            m_item = added_item;
        }
    }
};

// Inventory
class Inventory {
private:
    int m_rows;
    int m_columns;
    std::vector<std::vector<InventoryCell>> m_cells;
    int m_currentCell;

    Sprite m_temp_combined;

public:
    // Constructor
    Inventory(int rows, int columns, const Sprite& cellSprite)
        : m_rows(rows), m_columns(columns), m_currentCell(0) {
        m_cells.resize(rows, std::vector<InventoryCell>(columns, InventoryCell(cellSprite)));
    }

    void setCurrentCell(int index) {
        if (index >= 0 && index < m_rows * m_columns) {
            m_currentCell = index;
        }
    }
    int getCurrentCell() const {
        return m_currentCell;
    }
    void addItem(Item* item, int row, int column) {
        if (row >= 0 && row < m_rows && column >= 0 && column < m_columns) {
            m_cells[row][column].setItem(item);
        }
    }

    Sprite combineCellsToOneSprite() {
        int single_cell_width = m_cells[0][0].getCellSprite().getSpriteWidth();
        int single_cell_height = m_cells[0][0].getCellSprite().getSpriteHeight();

        std::vector<std::vector<char>> inventory_grid;

        Sprite combinedSprite = Sprite(0, single_cell_height * m_rows + 2, single_cell_width * m_columns + 2);
        
        for (int i = 0; i < m_columns; i++) 
        {
            for (int j = 0; i < m_rows; j++)
            {
                int c_w = m_rows % single_cell_width;
                int c_h = m_columns % single_cell_height;
                inventory_grid[i][j] = m_cells[0][0].getCellSprite().getFrame(0)[c_h][c_w];
            }
        }
        combinedSprite.addFrame(inventory_grid);

        return combinedSprite;
    }
};

// Player
class Player {
private:
    unsigned int m_player_health;
    double m_player_prev_y;
    double m_player_prev_x;
    double m_player_y;
    double m_player_x;
    double m_player_angle;
public:
    // Default constructor
    Player() {
        m_player_health = DEFAULT_PLAYER_HEALTH;
        m_player_y = DEFAULT_PLAYER_Y;
        m_player_x = DEFAULT_PLAYER_X;
        m_player_angle = DEFAULT_PLAYER_ANGLE;
        m_player_prev_y = m_player_x;
        m_player_prev_x = m_player_x;
    };
    //
    // Constructor
    Player(int player_health, double player_y, double player_x, double player_angle) {
        m_player_health = player_health;
        m_player_y = player_y;
        m_player_x = player_x;
        m_player_angle = player_angle;
        m_player_prev_y = m_player_x;
        m_player_prev_x = m_player_x;
    };
    //
    // Destructor
    ~Player() {

    };
    // 
    // Setters and getters
    void setPlayerHealth(int player_health) {
        m_player_health = player_health;
    }
    unsigned int getPlayerHealth() const {
        return m_player_health;
    }
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
    void setPlayerPrevY() {
        m_player_prev_y = m_player_y;
    }
    void setPlayerPrevX() {
        m_player_prev_x = m_player_x;
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
    //
    // Return player to previous position
    void returnBack() {
        m_player_x = m_player_prev_x;
        m_player_y = m_player_prev_y;
    }
    //
    // Applay damage to player
    void applyDamage(int damage) {
        if (m_player_health != 0) m_player_health -= damage;
    }


    //Lab 5
    unsigned int* getPlayerHealthPtr()
    {
        return &m_player_health;
    }
    unsigned int& getPlayerHealthRef()
    {
        return m_player_health;
    }
    // "Сложить" игрока с аптечкой, прибавить аптечку к здоровью
    friend Player operator+ (const Player& player, const FirstAidKit& kit);
};

Player operator+ (const Player& player, const FirstAidKit& kit) 
{
    Player newPlayer = player;
    newPlayer.setPlayerHealth(newPlayer.getPlayerHealth() + (kit.getAmount()*FIRST_AID_HP));
    return newPlayer;
}

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
    std::vector<std::shared_ptr<Enemy>> m_enemies;
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
    //
    // Add one enemy
    void addEnemy(std::shared_ptr<Enemy> enemy) {
        m_enemies.push_back(enemy);
    }
    //
    const std::vector<std::shared_ptr<Enemy>>& getEnemies() const {
        return m_enemies;
    }
};

void processPlayerCollision(Map& map) {
    char check = map.getObstacle(map.getPlayer().getPlayerY(), map.getPlayer().getPlayerX());
    if (check == '1' || check == '2') {
        map.getPlayer().returnBack();
    }
    else if (check == '3') {
        map.getPlayer().applyDamage(LAVA_DAMAGE);
    }
}

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
            std::cerr << "[ERROR]: Screen.drawText(): Starting position is out of screen bounds." << std::endl;
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
            std::cerr << "[ERROR]: Screen.drawDot(): wrong x" << std::endl;
            return;
        }
        if (y < 0 || y >= m_scr_height) {
            std::cerr << "[ERROR]: Screen.drawDot(): wrong y" << std::endl;
            return;
        }

        m_screen[y][x] = color;
    }
    //
    // Draws line with char color
    void drawLine(int x1, int y1, int x2, int y2, char color) {
        if (x1 < 0 || x1 >= m_scr_width) {
            std::cerr << "[ERROR]: Screen.drawLine(): Wrong x1" << std::endl;
            return;
        }
        if (x2 < 0 || x2 >= m_scr_width) {
            std::cerr << "[ERROR]: Screen.drawLine(): Wrong x2" << std::endl;
            return;
        }
        if (y1 < 0 || y1 >= m_scr_height) {
            std::cerr << "[ERROR]: Screen.drawLine(): Wrong y1" << std::endl;
            return;
        }
        if (y2 < 0 || y2 >= m_scr_height) {
            std::cerr << "[ERROR]: Screen.drawLine(): Wrong y2" << std::endl;
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
            std::cerr << "[ERROR]: Screen.drawRect(): Wrong x1" << std::endl;
            return;
        }
        if (x2 < 0 || x2 >= m_scr_width) {
            std::cerr << "[ERROR]: Screen.drawRect(): Wrong x2" << std::endl;
            return;
        }
        if (y1 < 0 || y1 >= m_scr_height) {
            std::cerr << "[ERROR]: Screen.drawRect(): Wrong y1" << std::endl;
            return;
        }
        if (y2 < 0 || y2 >= m_scr_height) {
            std::cerr << "[ERROR]: Screen.drawRect(): Wrong y2" << std::endl;
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
    // Draws dot with check but without errors
    void ffDrawDot(int y, int x, char color) {
        if (x < 0 || x >= m_scr_width) {
            return;
        }
        if (y < 0 || y >= m_scr_height) {
            return;
        }
        m_screen[y][x] = color;
    }
    //
    // Gets dot
    char fGetDot(int y, int x) {
        return (m_screen[y][x]);
    }
    //
    // Draws sprite with mask
    void drawSprite(int y, int x, int is_centered, int frame, Sprite& sprite, Screen& screen) {
        int spr_h = sprite.getSpriteHeight();
        int spr_w = sprite.getSpriteWidth();
        const auto& currentFrame = sprite.getFrame(frame);

        if (is_centered == 1) {
            y -= spr_h / 2;
            x -= spr_w / 2;
        }

        // Проверяем границы экрана
        for (int i = 0; i < spr_h; ++i) {
            for (int j = 0; j < spr_w; ++j) {
                int screenY = y + i;
                int screenX = x + j;

                if (screenY >= 0 && screenY < screen.m_scr_height && screenX >= 0 && screenX < screen.m_scr_width) {
                    if (currentFrame[i][j] != ' ') {
                        screen.m_screen[screenY][screenX] = currentFrame[i][j];
                    }
                }
            }
        }
    }
};

// Load map from file by path
Map loadMapFromFile(const char* filename) {
    std::ifstream file(filename);

    if (!file) {
        throw std::runtime_error("[ERROR]: loadMapFromFile(): Failed to open map file");
    }

    // Coords of player from file
    int player_health_file;
    double player_y_file;
    double player_x_file;
    double player_angle_file;

    // Read coords of the player
    file >> player_health_file >> player_y_file >> player_x_file >> player_angle_file;

    if (!file) {
        throw std::runtime_error("[ERROR]: loadMapFromFile(): Failed to read coords or angle of the player from file");
    }

    // New player with param from file
    Player player_file(player_health_file, player_y_file, player_x_file, player_angle_file);

    // Dimensions of the map from file
    int map_h_file;
    int map_w_file;

    // Read map dimensions
    file >> map_h_file >> map_w_file;

    if (!file) {
        throw std::runtime_error("[ERROR]: loadMapFromFile(): Failed to read map dimensions from file");
    }

    // New map with param from file
    Map map_file(map_h_file, map_w_file, player_file);

    // Read map's obstacle
    for (int i = 0; i < map_h_file; i++) {
        for (int j = 0; j < map_w_file; j++) {
            char obstacle_type;
            file >> obstacle_type;
            if (!file) {
                throw std::runtime_error("[ERROR]: loadMapFromFile(): Failed to read map data from file");
            }
            // Skip the comma
            char comma;
            file >> comma;
            map_file.setObstacle(i, j, obstacle_type);
        }
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::string type;
    std::string line;
    int enemy_health_file;
    double enemy_x_file, enemy_y_file;

    while (std::getline(file, type)) {
        if (std::getline(file, line)) {
            enemy_health_file = std::stod(line);
        }
        else {
            std::cerr << "[ERROR]: loadMapFromFile(): Failed to read enemy health for type: " << type << std::endl;
            break;
        }

        if (std::getline(file, line)) {
            enemy_y_file = std::stod(line);
        }
        else {
            std::cerr << "[ERROR]: loadMapFromFile(): Failed to read enemy Y coordinate for type: " << type << std::endl;
            break;
        }

        if (std::getline(file, line)) {
            enemy_x_file = std::stod(line);
        }
        else {
            std::cerr << "[ERROR]: loadMapFromFile(): Failed to read enemy X coordinate for type: " << type << std::endl;
            break;
        }

        if (type == "Goblin") {
            map_file.addEnemy(std::make_shared<Goblin>(enemy_health_file, enemy_y_file, enemy_x_file));
        }
        else if (type == "Orc") {
            map_file.addEnemy(std::make_shared<Orc>(enemy_health_file, enemy_y_file, enemy_x_file));
        }
        else {
            std::cerr << "[ERROR]: loadMapFromFile(): Unknown enemy type: " << type << std::endl;
        }
    }

    file.close();

    return map_file;
}

// Save map to file by path
void saveMap(const char* filename, const Map& map) {
    std::ofstream file(filename);

    if (!file) {
        throw std::runtime_error("[ERROR]: saveMap(): Failed to open map file for writing");
    }
    
    // Write player health
    file << map.getPlayer().getPlayerHealth() << std::endl;

    // Write coords of the player
    file << map.getPlayer().getPlayerY() << std::endl << map.getPlayer().getPlayerX() << std::endl << map.getPlayer().getPlayerAngle() << std::endl;

    // Write map dimensions
    file << map.getMapHeight() << std::endl << map.getMapWidth() << std::endl;

    // Write map's obstacles
    for (int i = 0; i < map.getMapHeight(); i++) {
        for (int j = 0; j < map.getMapWidth(); j++) {
            char obstacle_type = map.getObstacle(i, j);
            file << obstacle_type << ",";
        }
        file << std::endl;
    }

    for (const auto& enemy : map.getEnemies()) {
        std::string enemy_type = typeid(*enemy).name(); // Nmae
        if (enemy_type == typeid(Goblin).name()) {
            enemy_type = "Goblin";
        }
        else if (enemy_type == typeid(Orc).name()) {
            enemy_type = "Orc";
        }

        file << enemy_type << std::endl;
        file << enemy->getHealth() << std::endl;
        file << enemy->getY() << std::endl;
        file << enemy->getX() << std::endl;
    }

    file.close();
    std::cout << "[INFO]: the map has been successfully saved to file" << filename << std::endl;
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

    double prev_distance;

    for (int column = 0; column < rays_n; column++) {
        int is_corner = 0;

        int map_check_x = cam_x;
        int map_check_y = cam_y;

        int hit = 0;
        int hit_door = 0;

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
                char check = map.getObstacle(map_check_y, map_check_x);
                if (check == '1') {
                    hit = 1;
                    if ((fabs(dot_x - round(dot_x)) < 0.05) && (fabs(dot_y - round(dot_y)) < 0.05)) {
                        is_corner = 1;
                    }
                    break;
                }
                else if (check == '2') {
                    hit_door = 1;
                    break;
                }
                else if (check == '3') {  //lava
                    int floor_size = 1; double n_c = cur_distance * cos(ray_angle - cam_angle);
                    int ceiling = (double)(scr_h / 2) - (scr_h / n_c);
                    int floorr = scr_h - ceiling;
                    for (int i = floorr - floor_size; i <= floorr; i++) {
                        for (int j = column - floor_size; j <= column + floor_size; j++) {
                            screen.ffDrawDot(i, j, '#'); //lava
                        }
                    }
                }
                    
            }
        }

        prev_distance = cur_distance;
        if (hit == 1 || hit_door == 1) {


            char color; // color of wall
            // Corners of the wall blocks
            if (is_corner == 1) {
                color = '|';
            }
            else {
                int grad_size = sizeof(gradient);
                if (int(cur_distance) >= grad_size) {
                    color = gradient[0];
                }
                else {
                    int color_index = int((grad_size - cur_distance));
                    color = gradient[color_index];
                }
            }

            if (hit_door == 1) {
                cur_distance += 0.7;
                color = 'D';
            }

            double n_c = cur_distance * cos(ray_angle - cam_angle); // fish eye fix

            int ceiling = (double)(scr_h / 2) - (scr_h / n_c);
            int floorr = scr_h - ceiling;

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

        for (const auto& enemy : map.getEnemies()) {
            std::string enemy_type = typeid(*enemy).name(); // Nmae
            if (enemy_type == typeid(Goblin).name()) {
                enemy_type = "Goblin";
            }
            else if (enemy_type == typeid(Orc).name()) {
                enemy_type = "Orc";
            }

            double ey = enemy->getY();
            double ex = enemy->getX();
            

            // todo: enemy render
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

    // Draw the obstacle map, clipped to MINI_MAP_WIDTHxMINI_MAP_HEIGHT area around the playеer
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





