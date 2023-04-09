export module Game.World;

import <array>;
import <cmath>;
import <numbers>;
import <string>;

import Internal.GameStore;
import Game;
import Utils.Angle;
import Utils.BoundBox;
import Utils.Names;
import Utils.Random;

Sector *World::getSectorAt(const double x, const double y) {
    const int8_t sectorX = std::floor(x / this->config.sectorSize);
    const int8_t sectorY = std::floor(y / this->config.sectorSize);

    for (Sector &sector: this->sectors) {
        if (sector.x == sectorX && sector.y == sectorY) {
            return &sector;
        }
    }

    return nullptr;
}

std::array<Sector *, 9> World::getSectorsAround(const double x, const double y) {
    const int8_t sectorX = std::floor(x / this->config.sectorSize);
    const int8_t sectorY = std::floor(y / this->config.sectorSize);

    std::array < Sector * , 9 > result{nullptr};

    for (Sector &sector: this->sectors) {
        if (sector.x >= sectorX - 1 && sector.x <= sectorX + 1 &&
            sector.y >= sectorY - 1 && sector.y <= sectorY + 1) {
            result[(sector.y - sectorY + 1) * 3 + (sector.x - sectorX + 1)] = &sector;
        }
    }

    return result;
}

void World::addFood(Food *food) {
    auto *sector = this->getSectorAt(food->x, food->y);

    if (sector != nullptr) {
        sector->addFood(food);
    }
}

void World::removeFood(Food *food) {
    auto *sector = this->getSectorAt(food->x, food->y);

    if (sector != nullptr) {
        sector->removeFood(food);
    }
}

void World::addSnake(Snake *snake) {
    this->snakes[snake->id] = snake;

    if (snake->isPlayer) {
        this->player = snake;
    }
}

void World::removeSnake(Snake *snake) {
    this->snakes.erase(snake->id);

    if (snake->isPlayer) {
        this->player = nullptr;
    }

    delete snake;
}

Snake *World::createSnake(bool isBot, const std::string &username, bool isPlayer) {
    const auto distanceAngle = randomDouble(0, std::numbers::pi * 2);
    const auto distance = this->config.worldRadius * randomDouble(0.05, 0.5);
    auto x = std::cos(distanceAngle) * distance;
    auto y = std::sin(distanceAngle) * distance;

    // prevent the snakes from being spawned too close to each other
    for (const auto &[snakeId, snake]: this->snakes) {
        if (snake->head.getBoundBox(snake).isInclude(x, y)) {
            // too close!
            return this->createSnake(isBot, username, isPlayer);
        }
    }

    auto angle = randomDouble(0, std::numbers::pi * 2); // the angle to spawn the body to
    auto snake = new Snake{
        Snake::nextSnakeId++,
        x, y, reverseAngle(angle),
        isBot, isPlayer,
        username
    };

    // head is already saved in a separate variable
    for (size_t i = 1; i < 7; i++) {
        x += std::cos(angle) * Snake::stepDistance;
        y += std::sin(angle) * Snake::stepDistance;

        snake->bodyParts.emplace_back(x, y);
    }

    snake->updateStatus();

    return snake;
}

/**
 * Fill the world with bot snakes if there are not enough players
 */
void World::fillSnake() {
    const auto snakeNeeded = this->config.targetSnakeCount - this->snakes.size();

    for (size_t i = 0; i < snakeNeeded; i++) {
        this->addSnake(this->createSnake(true, "电脑玩家" + getNextName()));
    }
}

void World::initSectors() {
    const auto sectorCount = static_cast<size_t>(std::pow(this->config.sectorCountEdge, 2));
    this->sectors.reserve(sectorCount);

    for (size_t i = 0; i < sectorCount; i++) {
        Sector sector{
            config,
            static_cast<int8_t>(i % this->config.sectorCountEdge - this->config.sectorCountEdge / 2),
            static_cast<int8_t>(i / this->config.sectorCountEdge - this->config.sectorCountEdge / 2)
        };

        if (sector.isIntersect(*this)) {
            this->sectors.push_back(sector);
        }
    }
}
