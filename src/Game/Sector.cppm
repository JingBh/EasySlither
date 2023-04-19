export module Game.Sector;

import <cmath>;

import Game;
import Utils.Random;
import Utils.Throttle;

void Sector::addFood(Food *food) {
    this->foods[food->id] = food;
}

void Sector::removeFood(Food *food) {
    this->foods.erase(food->id);
    delete food;
}

Food *Sector::generateFood() {
    const double foodX = (this->x + 0.5) * this->config.sectorSize
                         + randomDouble(this->config.sectorSize / 2.0);
    const double foodY = (this->y + 0.5) * this->config.sectorSize
                         + randomDouble(this->config.sectorSize / 2.0);

    return new Food{
        static_cast<int>(foodY) * config.worldRadius * 3 + static_cast<int>(foodX),
        foodX, foodY,
        static_cast<uint8_t>(randomInt(2, 10))
    };
}

void Sector::fillFood(const bool gradually) {
    const auto foodCount = this->foods.size();

    if (foodCount >= this->targetFoodDensity) {
        return;
    }

    if (!gradually) {
        for (uint16_t i = 0; i < this->targetFoodDensity - foodCount; i++) {
            this->addFood(this->generateFood());
        }
        return;
    }

    this->addFood(this->generateFood());
}

uint16_t Sector::distanceTo(const Sector &other) const {
    return std::floor(std::hypot(this->x - other.x, this->y - other.y));
}

uint16_t Sector::distanceToCenter() const {
    if (this->x == 0 && this->y == 0) {
        return 0; // to prevent stack overflow
    }

    return this->distanceTo(Sector{this->config, 0, 0});
}
