export module Game.Snake;

import <array>;
import <cmath>;
import <cstdint>;
import <numbers>;
import <limits>;

import Game;
import Internal.GameStore;
import Utils.Angle;
import Utils.BoundBox;
import Utils.Random;

constexpr std::array<double, 7> snakePartsDistance = {
    42, 42, 42, 37.7, 37.7, 33, 28.5
};
constexpr auto snakeHeadDistanceSum = 42 + 42 + 42 + 37.7 + 37.7 + 33 + 28.5;
constexpr double tailStepDistance = 24;

[[nodiscard]] size_t Snake::getLength() const {
    return this->bodyParts.size() + 1;
}

[[nodiscard]] bool Snake::canBoost() const {
    return this->getLength() > Snake::headLength;
}

void Snake::grow(const double amount) {
    this->fullness += amount;
    while (this->fullness >= 1) {
        this->fullness -= 1;
        this->bodyParts.push_back(this->bodyParts.back());
    }
}

void Snake::shrink(const double amount) {
    if (amount > this->fullness) {
        this->fullness += 1;

        const size_t loseParts = std::ceil(amount);
        for (size_t i = 0; i < loseParts; i++) {
            if (this->getLength() > Snake::headLength) {
                const auto &last = this->bodyParts.back();

                auto *world = GameStore::getInstance()->getWorld();
                auto *food = new Food{
                    world->config,
                    static_cast<int16_t>(last.x),
                    static_cast<int16_t>(last.y),
                    static_cast<uint8_t>(randomInt(10, 50))
                };
                world->addFood(food);

                this->bodyParts.pop_back();
            }
        }
    } else {
        this->fullness -= amount;
    }
}

void Snake::move(unsigned long long timeSpan) {
    const double ticks = static_cast<double>(timeSpan) / 8; // tick time 8ms
    double rotation = Snake::speedAngularBase * ticks;

    // check boost
    if (this->isBoost) {
        if (this->canBoost()) {
            this->shrink(0.015 * ticks);
        } else {
            this->isBoost = false;
        }
    }

    // rotation
    if (this->angle != this->wAngle) {
        double angleChangeWanted = normalizeAngle(this->wAngle - this->angle);

        if (rotation >= std::abs(angleChangeWanted)) {
            this->angle = this->wAngle;
        } else {
            this->angle = rotateAngle(this->angle, angleChangeWanted > 0 ? rotation : -rotation);
        }
    }

    // movement
    {
        const double speed = this->isBoost ? Snake::speedLinearBoost : Snake::speedLinearBase;
        const double distance = speed * ticks;

        SnakeBody previousPart = this->head;

        // move head
        this->head.x += std::cos(this->angle) * distance;
        this->head.y += std::sin(this->angle) * distance;

        // move body parts
        for (size_t i = 0; i < this->getLength() - 1; i++) {
            auto &bodyPart = this->bodyParts[i];

            // const double distanceToPrevious = i < 7 ? snakePartsDistance[i] : tailStepDistance;
            const double distanceToPrevious = tailStepDistance;
            const double angleToPrevious = std::atan2(previousPart.y - bodyPart.y, previousPart.x - bodyPart.x);

            bodyPart.x = previousPart.x - std::cos(angleToPrevious) * distanceToPrevious;
            bodyPart.y = previousPart.y - std::sin(angleToPrevious) * distanceToPrevious;

            previousPart = bodyPart;
        }
    }
}

void Snake::updateStatus() {
    this->scale = std::fmin(6, 1 + (this->getLength() - 1) / 106);
    this->speedAngularDeltaThickness = 0.13 + 0.87 * std::pow((7 - this->scale) / 6, 2);
    this->bodyPartRadius = 29 * 0.5 * this->scale;

    double centerX = this->head.x, centerY = this->head.y;
    for (auto &bodyPart: this->bodyParts) {
        centerX += bodyPart.x;
        centerY += bodyPart.y;
    }
    centerX /= static_cast<double>(this->getLength());
    centerY /= static_cast<double>(this->getLength());

    double zoneDistance = snakeHeadDistanceSum +
                          static_cast<double>(this->getLength() - 7) * tailStepDistance;

    this->zone = HasRoundBoundBox{
        centerX, centerY,
        zoneDistance / 2
    };
}

void Snake::checkFoodEaten() {
    auto *world = GameStore::getInstance()->getWorld();
    if (world == nullptr) return;
    auto *sector = world->getSectorAt(this->head.x, this->head.y);
    if (sector == nullptr) return;

    auto headBoundBox = this->getHeadBoundBox();
    headBoundBox.boundBoxRadius *= 1.5;

    // foods might change during iteration
    // make copy of the original map
    auto foods = sector->foods;
    for (auto &[foodId, food]: foods) {
        if (food == nullptr) return;
        if (headBoundBox.isInclude(food->x, food->y)) {
            this->grow(food->size / 100.0);
            sector->removeFood(food);
        }
    }
}

uint16_t Snake::getScore() const {
    const auto length = static_cast<long double>(this->getLength()) + this->fullness;

    // fitted function:
    // y = 1.9160842554E-10x6 - 1.4564505459E-07x5 + 4.3828621608E-05x4 - 5.9629462725E-03x3 + 4.2820519417E-01x2 + 5.6554831974E+00x
    return static_cast<uint16_t>(std::fmin(std::fmax(std::floor(
        1.9160842554e-10 * std::pow(length, 6) -
        1.4564505459e-07 * std::pow(length, 5) +
        4.3828621608e-05 * std::pow(length, 4) -
        5.9629462725e-03 * std::pow(length, 3) +
        4.2820519417e-01 * std::pow(length, 2) +
        5.6554831974e+00 * length
    ), 0), std::numeric_limits<uint16_t>::max()));
}

HasRoundBoundBox Snake::getHeadBoundBox() const {
    return HasRoundBoundBox{
        this->head.x,
        this->head.y,
        this->bodyPartRadius
    };
}
