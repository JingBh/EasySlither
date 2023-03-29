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

import Game.Config;
import Utils.BoundBox;

export class World;

export class Sector;

export class Food;

export class Snake;

export class SnakeBody;

class Food {
public:
    const int32_t id;
    const int16_t x, y;
    const uint8_t size;

public:
    Food(const GameConfig config,
         const int16_t x, const int16_t y, const uint8_t size)
        : id{y * config.worldRadius * 3 + x}, x{x}, y{y}, size{size} {}
};

class SnakeBody {
public:
    double x, y;

    SnakeBody(const double x, const double y)
        : x{x}, y{y} {}
};

class Snake {
public:
    const int32_t id;
    const bool isBot;
    const bool isPlayer;
    const std::string username;

private:
    SnakeBody head;
    std::deque <SnakeBody> bodyParts;
    double fullness = 0;

public:
    Snake(const int32_t id,
          const double x, const double y,
          const bool isBot, const bool isPlayer,
          std::string_view username)
        : id{id}, head{x, y}, isBot{isBot}, isPlayer{isPlayer}, username{username} {}
};

class Sector : public HasRoundBoundBox {
public:
    const uint8_t x, y;
    const GameConfig config;

private:
    const uint16_t targetFoodDensity;
    std::map<int32_t, Food *> foods;

public:
    Sector(const GameConfig config,
           const uint8_t x, const uint8_t y)
        : x{x}, y{y},
          HasRoundBoundBox{
              config.sectorSize * (x + 0.5),
              config.sectorSize * (y + 0.5),
              (config.sectorSize * std::numbers::sqrt2 + 1) / 2
          },
          targetFoodDensity{
              // this is calculated based on the sector's distance to the center
              // the closer to the center, the more food it will have
              static_cast<uint16_t>((1 - this->distanceToCenter() / std::pow(config.sectorCountEdge, 2)) *
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

class World {
public:
    const GameConfig config; // the config used to create this world

private:
    std::vector <Sector> sectors;
    std::map<int32_t, Food *> foods;
    std::map<int32_t, Snake *> snakes;
    Snake *player = nullptr;

public:
    explicit World(const GameConfig config)
        : config{config} {
        /**
         * sectors initialization
         */
        const auto sectorCount = static_cast<size_t>(std::pow(this->config.sectorCountEdge, 2));
        this->sectors.reserve(sectorCount);

        for (size_t i = 0; i < sectorCount; i++) {
            this->sectors.emplace_back(
                config,
                static_cast<int8_t>(i % this->config.sectorCountEdge),
                static_cast<int8_t>(i / this->config.sectorCountEdge)
            );
        }
    }

    void addSnake(Snake *snake);

    void removeSnake(Snake *snake);
};
