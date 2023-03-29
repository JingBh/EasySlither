export module Game.Sector;

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
    return new Food{
        this->config,
        static_cast<int16_t>(this->x * this->config.sectorSize
                             + randomInt(this->config.sectorSize)),
        static_cast<int16_t>(this->y * this->config.sectorSize
                             + randomInt(this->config.sectorSize)),
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

    // TODO: make this better
    this->addFood(this->generateFood());
}

uint16_t Sector::distanceTo(const Sector &other) const {
    return static_cast<uint16_t>(std::pow(std::abs(this->x - other.x), 2)
                                 + std::pow(std::abs(this->y - other.y), 2));
}

uint16_t Sector::distanceToCenter() const {
    const uint8_t center = this->config.sectorCountEdge / 2;
    return this->distanceTo(Sector{center, center, this->config});
}
