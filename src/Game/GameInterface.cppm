/**
 * Interface of the Game module.
 */

export module Game;

import <cmath>;
import <cstdint>;
import <deque>;
import <map>;
import <numbers>;
import <string>;
import <vector>;

import Game.GameConfig;
import Utils.BoundBox;

export class World;

export class Sector;

export class Food;

export class Snake;

export class SnakeBody;

export class Prey;

class Food {
public:
    const int32_t id;
    double x, y;
    const uint8_t size;

    Food(const int32_t id, const double x, const double y, const uint8_t size)
        : id{id}, x{x}, y{y}, size{size} {}
};

class Prey : public Food {
public:
    double angle, wAngle;

private:
    static constexpr double speedAngularBase = 0.028; // mamu2
    static constexpr double speedLinearBase = 1.88;
    static inline int32_t nextPreyId = 0; // auto-increment id

public:
    Prey(const double x, const double y, const uint8_t size, const double angle)
        : Food(nextPreyId++, x, y, size), angle{angle}, wAngle{angle} {}

    void turn();

    void move(unsigned long long timeSpan);
};

class SnakeBody {
public:
    double x, y;

    SnakeBody(const double x, const double y)
        : x{x}, y{y} {}

    HasRoundBoundBox getBoundBox(const Snake *snake) const;
};

class Snake {
    friend class World;

public:
    const int32_t id;
    const bool isBot;
    const bool isPlayer;
    const std::string username;
    SnakeBody head;
    std::deque <SnakeBody> bodyParts;
    bool isBoost = false;
    bool isDying = false;
    int8_t deadTicks = -101;
    double angle, wAngle; // ang, wang
    double bodyPartRadius;
    HasRoundBoundBox zone{0, 0, 0}; // the area where the snake is at

private:
    double fullness = 0;
    double scale; // sc
    double fullnessThreshold = 1;
    double speedAngularCoefficientThickness; // scang
    double speedAngularCoefficientVelocity; // spang

    static constexpr uint8_t headLength = 3;
    static constexpr uint16_t stepDistance = 42;
    static constexpr double speedAngularBase = 1.0 / 30; // mamu
    static constexpr double speedLinearBase = 1.48;
    static constexpr double speedLinearBoost = 3.584;
    static inline int32_t nextSnakeId = 0; // auto-increment id

public:
    Snake(const int32_t id,
          const double x, const double y, const double angle,
          const bool isBot, const bool isPlayer,
          std::string_view username)
        : id{id},
          head{x, y}, angle{angle}, wAngle{angle},
          isBot{isBot}, isPlayer{isPlayer},
          username{username} {}

    [[nodiscard]] size_t getLength() const;

    [[nodiscard]] bool canBoost() const;

    void grow(const double amount);

    void shrink(const double amount);

    void move(unsigned long long timeSpan);

    void updateStatus();

    void checkFoodEaten();

    void setDying();

    void turnIntoFood();

    void tickAI();

    uint32_t getScore() const;

    HasRoundBoundBox getHeadTipBoundBox() const;
};

class Sector : public HasRectBoundBox {
    friend class World;

public:
    const int8_t x, y;
    const GameConfig config;
    std::map<int32_t, Food *> foods;

private:
    const uint16_t targetFoodDensity;

public:
    Sector(const GameConfig config,
           const int8_t x, const int8_t y)
        : x{x}, y{y},
          HasRectBoundBox{
              static_cast<double>(config.sectorSize * x),
              static_cast<double>(config.sectorSize * y),
              static_cast<double>(config.sectorSize * (x + 1)),
              static_cast<double>(config.sectorSize * (y + 1))
          },
          targetFoodDensity{
              // this is calculated based on the sector's distance to the center
              // the closer to the center, the more food it will have
              static_cast<uint16_t>((1 - this->distanceToCenter() / config.sectorCountEdge) *
                                    config.foodDensity)
          },
          config{config} {
        this->fillFood(false);
    }

    void addFood(Food *food);

    void removeFood(Food *food);

    Food *generateFood();

    void fillFood(const bool gradually = true);

protected:
    [[nodiscard]] uint16_t distanceTo(const Sector &other) const;

    [[nodiscard]] uint16_t distanceToCenter() const;
};

class World : public HasRoundBoundBox {
public:
    const GameConfig config; // the config used to create this world
    std::vector <Sector> sectors;
    std::map<int32_t, Snake *> snakes;
    std::map<int32_t, Prey *> preys;
    Snake *player = nullptr;

public:
    explicit World(const GameConfig config)
        : config{config}, HasRoundBoundBox(0, 0, config.worldRadius) {
        this->initSectors();
    }

    Sector *getSectorAt(const double x, const double y);

    std::array<Sector *, 9> getSectorsAround(const double x, const double y);

    void addFood(Food *food);

    void removeFood(Food *food);

    void addSnake(Snake *snake);

    void removeSnake(Snake *snake);

    void addPrey(Prey *prey);

    void removePrey(Prey *prey);

    Snake *createSnake(bool isBot, const std::string &username, bool isPlayer = false);

    void fillSnake();

    Prey *createPrey();

    void fillPreys();

private:
    void initSectors();
};
