export module Game.World;

import <cmath>;
import <numbers>;
import <string>;

import Game;
import Utils.Angle;
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
    delete snake;
}

Snake *World::createSnake(bool isBot, const std::string &username, bool isPlayer) {
    const auto distanceAngle = randomDouble(0, std::numbers::pi * 2);
    const auto distance = this->config.worldRadius * randomDouble(0.05, 0.5);
    auto x = std::cos(distanceAngle) * distance;
    auto y = std::sin(distanceAngle) * distance;

#ifdef DEBUG
    if (isPlayer) {
        x = 0;
        y = 0;
    }
#endif //DEBUG

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
