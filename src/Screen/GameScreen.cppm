export module Screen.GameScreen;

import <array>;
import <cmath>;
import <numbers>;
import <memory>;
import <mutex>;
import <string>;

import Game;
import Internal.UserInput;
import Screen;
import Screen.ScaleFactor;
import ThirdParty;
import Utils.BoundBox;
import Utils.Colors;
import Utils.TextEncode;

[[nodiscard]] std::unique_ptr <easyx::Image> GameScreen::renderImage() const {
    const auto width = this->screen.getWidth();
    const auto height = this->screen.getHeight();
    const auto minSide = std::min(width, height);

    auto image = std::make_unique<easyx::Image>(width, height);

    easyx::setWorkingImage(image.get());
    easyx::setOrigin(0, height); // set bottom-left point as origin
    easyx::setAspectRatio(1, -1); // and set upwards as y-axis direction
    easyx::setBackgroundStyle(true);
    easyx::setBackgroundColor(BLACK);
    easyx::clearDevice();

    if (this->store->getWorld() == nullptr) {
        // still initializing, wait
        std::unique_lock <std::mutex> ul(this->store->worldInitializationMutex);
        this->store->worldInitialized.wait(ul);
    }

    this->store->worldMutex.lock();

    auto *world = this->store->getWorld();

    // const auto scale = static_cast<float>((std::pow(1.25, zoom) * minSide / (world->config.worldRadius * 2))
    //     * SCALE_FACTOR);

    // easyx::setAspectRatio(scale, -scale);

    auto *player = world->player;
    const auto headX = player->head.x;
    const auto headY = player->head.y;
    HasRectBoundBox viewport = {
        headX - static_cast<double>(width) / 2,
        headY - static_cast<double>(height) / 2,
        headX + static_cast<double>(width) / 2,
        headY + static_cast<double>(height) / 2
    };

    easyx::setLineStyle(1);
    easyx::setLineColor(BLACK);
    for (auto &sector: world->sectors) {
        // check if sector is in sight
        if (viewport.isIntersect(sector)) {
            // start draw sector
            easyx::setFillColor(GRAY_900);
            easyx::drawRectangle(
                static_cast<int>(sector.boundBoxX1 - viewport.boundBoxX1),
                static_cast<int>(sector.boundBoxY1 - viewport.boundBoxY1),
                static_cast<int>(sector.boundBoxX2 - viewport.boundBoxX1),
                static_cast<int>(sector.boundBoxY2 - viewport.boundBoxY1));

            // start draw foods
            easyx::setFillColor(EMBER_700);
            for (auto &[foodId, food]: sector.foods) {
                // check if food is in sight
                if (viewport.isInclude(food->x, food->y)) {
                    easyx::drawCircle(
                        static_cast<int>(food->x - viewport.boundBoxX1),
                        static_cast<int>(food->y - viewport.boundBoxY1),
                        static_cast<int>(5 * std::log10(food->size)),
                        false);
                }
            }
        }
    }

    for (auto &[snakeId, snake]: world->snakes) {
        // check if snake is in sight
        if (!viewport.isIntersect(snake->zone)) {
            continue;
        }

        // if sneak is already dead, skip rendering
        if (snake->isDying && snake->deadTicks <= 0) {
            continue;
        }

        // start draw snake
        easyx::setFillColor(snake->isPlayer ? CYAN_700 : EMERALD_700);
        if (snake->isDying || snake->isBoost) {
            easyx::setLineStyle(2);
            easyx::setLineColor(snake->isPlayer ? CYAN_300 : EMERALD_300);
        } else {
            easyx::setLineStyle(1);
            easyx::setLineColor(GRAY_800);
        }

        // draw snake body
        for (auto it = snake->bodyParts.rbegin(), end = snake->bodyParts.rend(); it != end; ++it) {
            SnakeBody *nextPart = &snake->head;
            auto itNext = std::next(it);
            if (itNext != end) {
                nextPart = &*itNext;
            }

            // interpolate a value to make the snake look smoother
            SnakeBody interpolate{
                it->x + (nextPart->x - it->x) / 2,
                it->y + (nextPart->y - it->y) / 2
            };

            std::array < SnakeBody * , 2 > toDraw{&*it, &interpolate};
            for (auto &part: toDraw) {
                easyx::drawCircle(
                    static_cast<int>(part->x - viewport.boundBoxX1),
                    static_cast<int>(part->y - viewport.boundBoxY1),
                    static_cast<int>(snake->bodyPartRadius));
            }
        }

        // draw snake head
        easyx::drawCircle(
            static_cast<int>(snake->head.x - viewport.boundBoxX1),
            static_cast<int>(snake->head.y - viewport.boundBoxY1),
            static_cast<int>(snake->bodyPartRadius));

        {
            // draw snake eyes
            easyx::setLineStyle(0);
            easyx::setFillColor(GRAY_100);
            double eyeAngle = snake->angle + std::numbers::pi / 5;
            double eyeX = snake->head.x + std::cos(eyeAngle) * snake->bodyPartRadius / 3 * 2;
            double eyeY = snake->head.y + std::sin(eyeAngle) * snake->bodyPartRadius / 3 * 2;
            easyx::drawCircle(
                static_cast<int>(eyeX - viewport.boundBoxX1),
                static_cast<int>(eyeY - viewport.boundBoxY1),
                static_cast<int>(snake->bodyPartRadius / 3));

            easyx::setFillColor(BLACK);
            double pupilX = eyeX + std::cos(snake->wAngle) * snake->bodyPartRadius / 6;
            double pupilY = eyeY + std::sin(snake->wAngle) * snake->bodyPartRadius / 6;
            easyx::drawCircle(
                static_cast<int>(pupilX - viewport.boundBoxX1),
                static_cast<int>(pupilY - viewport.boundBoxY1),
                static_cast<int>(snake->bodyPartRadius / 6));

            easyx::setFillColor(GRAY_100);
            eyeAngle = snake->angle - std::numbers::pi / 5;
            eyeX = snake->head.x + std::cos(eyeAngle) * snake->bodyPartRadius / 3 * 2;
            eyeY = snake->head.y + std::sin(eyeAngle) * snake->bodyPartRadius / 3 * 2;
            easyx::drawCircle(
                static_cast<int>(eyeX - viewport.boundBoxX1),
                static_cast<int>(eyeY - viewport.boundBoxY1),
                static_cast<int>(snake->bodyPartRadius / 3));

            easyx::setFillColor(BLACK);
            pupilX = eyeX + std::cos(snake->wAngle) * snake->bodyPartRadius / 6;
            pupilY = eyeY + std::sin(snake->wAngle) * snake->bodyPartRadius / 6;
            easyx::drawCircle(
                static_cast<int>(pupilX - viewport.boundBoxX1),
                static_cast<int>(pupilY - viewport.boundBoxY1),
                static_cast<int>(snake->bodyPartRadius / 6));
        }

        if (!snake->isPlayer) {
            // draw snake name
            easyx::setFont(12);
            easyx::setBackgroundStyle(false);
            easyx::setTextColor(GRAY_500);

            windows::Rect location{
                static_cast<long>(snake->head.x - 50 - viewport.boundBoxX1),
                static_cast<long>(snake->head.y + 20 - viewport.boundBoxY1),
                static_cast<long>(snake->head.x + 50 - viewport.boundBoxX1),
                static_cast<long>(snake->head.y + 32 - viewport.boundBoxY1)
            };
            easyx::drawText(encode(snake->username),
                            &location,
                            easyx::TEXT_CENTER | easyx::TEXT_VCENTER | easyx::TEXT_WORD_ELLIPSIS);
        }
    }

    easyx::setFont(16);
    easyx::setBackgroundStyle(false);
    easyx::setTextColor(GRAY_500);

    windows::Rect location{8,
                           height - 24,
                           width,
                           height - 8};
    easyx::drawText(encode("当前分数：" + std::to_string(player->getScore())),
                    &location,
                    easyx::TEXT_LEFT | easyx::TEXT_VCENTER);

#ifdef DEBUG
    location = {0,
                height - 24,
                width - 8,
                height - 8};
    easyx::drawText(encode("X=" + std::to_string(player->head.x) +
                           ", Y=" + std::to_string(player->head.y) +
                           ", angle=" + std::to_string(player->angle / std::numbers::pi) + "π" +
                           ", wAngle=" + std::to_string(player->wAngle / std::numbers::pi) + "π" +
                           ", isBoost=" + std::to_string(player->isBoost)),
                    &location,
                    easyx::TEXT_RIGHT | easyx::TEXT_VCENTER);
#endif //DEBUG

    this->store->worldMutex.unlock();

    easyx::setWorkingImage();
    easyx::setOrigin(0, 0);
    easyx::setAspectRatio(SCALE_FACTOR, SCALE_FACTOR);

    return image;
}

void GameScreen::onInputDirection(int degree) {
    auto *world = this->store->getWorld();

    if (world != nullptr) {
        auto angle = (degree - 90) / 180.0 * std::numbers::pi;

        world->player->wAngle = angle;
    }
}

void GameScreen::onMouseMove(const windows::Point &point) {
    auto *world = this->store->getWorld();

    if (world != nullptr) {
        const auto width = this->screen.getWidth();
        const auto height = this->screen.getHeight();

        const auto x = point.x - width / 2;
        const auto y = point.y - height / 2;

        world->player->wAngle = std::atan2(y, x);
    }
}

void GameScreen::onKeyHold(bool hold) {
    auto *world = this->store->getWorld();

    if (world != nullptr) {
        world->player->isBoost = hold && world->player->canBoost();
    }
}
