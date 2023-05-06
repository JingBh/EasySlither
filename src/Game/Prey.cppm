export module Game.Prey;

import <cmath>;
import <numbers>;

import Game;
import Internal.GameStore;
import Utils.Angle;
import Utils.Random;

void Prey::turn() {
    const auto *world = GameStore::getInstance()->getWorld();

    // the more the prey is away from the center, the more it tends to go back
    const auto distanceToCenter = std::hypot(this->x, this->y);
    const auto tendency = std::min(1.0, distanceToCenter / world->config.worldRadius / 0.9);

    const auto angleToCenter = std::atan2(-this->y, -this->x);
    this->wAngle = rotateAngle(angleToCenter, randomDouble((1 - tendency) * std::numbers::pi));
}

void Prey::move(unsigned long long timeSpan) {
    const double ticks = static_cast<double>(timeSpan) / 8; // tick time 8ms

    // rotation
    if (this->angle != this->wAngle) {
        const auto rotation = Prey::speedAngularBase * ticks;
        const auto angleChangeWanted = normalizeAngle(this->wAngle - this->angle);

        if (rotation >= std::abs(angleChangeWanted)) {
            this->angle = this->wAngle;
        } else {
            this->angle = rotateAngle(this->angle, angleChangeWanted > 0 ? rotation : -rotation);
        }
    }

    // movement
    const auto distance = Prey::speedLinearBase * ticks;
    this->x += std::cos(this->angle) * distance;
    this->y += std::sin(this->angle) * distance;
}
