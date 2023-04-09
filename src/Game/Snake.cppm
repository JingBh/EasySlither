export module Game.Snake;

import <array>;
import <cmath>;
import <cstdint>;
import <numbers>;
import <vector>;

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

static std::vector<double> snakePartScore;
static std::vector<double> snakeScoreBase{0};

[[nodiscard]] size_t Snake::getLength() const {
    return this->bodyParts.size() + 1;
}

[[nodiscard]] bool Snake::canBoost() const {
    return this->getLength() > Snake::headLength;
}

void Snake::grow(const double amount) {
    const auto *world = GameStore::getInstance()->getWorld();

    this->fullness += amount;

    while (this->fullness >= this->fullnessThreshold) {
        if (this->getLength() >= world->config.maxBodyParts) {
            break;
        }

        this->fullness -= this->fullnessThreshold;
        this->bodyParts.push_back(this->bodyParts.back());

        this->updateStatus();
    }
}

void Snake::shrink(const double amount) {
    auto *world = GameStore::getInstance()->getWorld();
    auto &last = this->bodyParts.back();

    if (amount < 0.1) {
        const auto minFoodSize = static_cast<int>(std::max(0.01, amount / 5) * 100);
        const auto maxFoodSize = static_cast<int>(std::max(0.02, amount / 2) * 100);

        auto *food = new Food{
            world->config,
            static_cast<int16_t>(last.x + randomDouble(this->bodyPartRadius)),
            static_cast<int16_t>(last.y + randomDouble(this->bodyPartRadius)),
            static_cast<uint8_t>(randomInt(minFoodSize, maxFoodSize))
        };

        world->addFood(food);
    } else {
        size_t generateFoods = std::floor(amount / 0.1);

        for (size_t i = 0; i < generateFoods; i++) {
            auto *food = new Food{
                world->config,
                static_cast<int16_t>(last.x + randomDouble(this->bodyPartRadius)),
                static_cast<int16_t>(last.y + randomDouble(this->bodyPartRadius)),
                static_cast<uint8_t>(randomInt(2, 5))
            };

            world->addFood(food);
        }
    }

    while (amount > this->fullness) {
        this->fullness += this->fullnessThreshold;

        const size_t loseParts = std::ceil(amount);
        for (size_t i = 0; i < loseParts; i++) {
            if (this->getLength() > Snake::headLength) {
                this->bodyParts.pop_back();
            }
        }

        this->updateStatus();
    }

    this->fullness -= amount;
}

void Snake::move(unsigned long long timeSpan) {
    const double ticks = static_cast<double>(timeSpan) / 8; // tick time 8ms

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
        const double speedAngular = Snake::speedAngularBase *
                                    this->speedAngularCoefficientThickness *
                                    this->speedAngularCoefficientVelocity;
        const double rotation = speedAngular * ticks;
        const double angleChangeWanted = normalizeAngle(this->wAngle - this->angle);

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
        for (auto &it: this->bodyParts) {
            auto *bodyPart = &it;

            // const double distanceToPrevious = i < 7 ? snakePartsDistance[i] : tailStepDistance;
            const double distanceToPrevious = tailStepDistance;
            const double angleToPrevious = std::atan2(previousPart.y - bodyPart->y, previousPart.x - bodyPart->x);

            bodyPart->x = previousPart.x - std::cos(angleToPrevious) * distanceToPrevious;
            bodyPart->y = previousPart.y - std::sin(angleToPrevious) * distanceToPrevious;

            previousPart = *bodyPart;
        }
    }
}

void Snake::updateStatus() {
    this->scale = std::fmin(6, 1 + (static_cast<double>(this->getLength() - 1)) / 106);
    this->fullnessThreshold = std::pow(this->scale, 2);
    this->speedAngularCoefficientThickness = 0.13 + 0.87 * std::pow((7 - this->scale) / 6, 2);
    this->speedAngularCoefficientVelocity = this->isBoost ?
                                            Snake::speedLinearBoost / Snake::speedLinearBase : 1;
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

    auto headBoundBox = this->head.getBoundBox(this);
    headBoundBox.boundBoxRadius *= 1.25;

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

void Snake::setDying() {
    this->isDying = true;
    this->deadTicks = 25;
}

void Snake::turnIntoFood() {
    auto *world = GameStore::getInstance()->getWorld();
    std::vector < SnakeBody * > allBodyParts{&this->head};

    for (auto &bodyPart: this->bodyParts) {
        allBodyParts.push_back(&bodyPart);
    }

    for (auto &bodyPart: allBodyParts) {
        for (size_t i = 0; i < 10; i++) {
            auto *food = new Food{
                GameStore::getInstance()->getWorld()->config,
                static_cast<int16_t>(bodyPart->x + randomDouble(this->bodyPartRadius)),
                static_cast<int16_t>(bodyPart->y + randomDouble(this->bodyPartRadius)),
                static_cast<uint8_t>(randomInt(5, 15))
            };

            world->addFood(food);
        }
    }
}

uint32_t Snake::getScore() const {
    const auto *world = GameStore::getInstance()->getWorld();
    const auto bodyPartCount = this->bodyParts.size();

    for (size_t i = snakePartScore.size(); i <= bodyPartCount; i++) {
        const double thisData = std::pow(1 - static_cast<double>(i) / world->config.maxBodyParts, 2.25);
        snakePartScore.push_back(thisData);
    }

    for (size_t i = snakeScoreBase.size(); i <= bodyPartCount; i++) {
        const double thisData = snakeScoreBase[i - 1] + 1 / snakePartScore[i - 1];
        snakeScoreBase.push_back(thisData);
    }

    const auto fullnessScore = this->fullness / snakePartScore[bodyPartCount];
    const auto score = std::floor(15 * (snakeScoreBase[bodyPartCount] + fullnessScore - 1) - 5);
    return static_cast<uint32_t>(score);
}

HasRoundBoundBox Snake::getHeadTipBoundBox() const {
    auto boundBox = this->head.getBoundBox(this);

    boundBox.boundBoxX += std::cos(this->angle) * this->bodyPartRadius / 2;
    boundBox.boundBoxY += std::sin(this->angle) * this->bodyPartRadius / 2;
    boundBox.boundBoxRadius = this->bodyPartRadius / 2;

    return boundBox;
}

HasRoundBoundBox SnakeBody::getBoundBox(const Snake *snake) const {
    return HasRoundBoundBox{
        this->x,
        this->y,
        snake->bodyPartRadius
    };
}
