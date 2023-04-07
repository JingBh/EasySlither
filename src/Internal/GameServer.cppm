export module Internal.GameServer;

import <cstdint>;
import <cstdio>;
import <list>;
import <memory>;
import <mutex>;
import <thread>;

import Game;
import Game.Config;
import Internal.GameStore;
import Network.Connection;
import Utils.Frames;

export class GameServer {
private:
    std::list <IConnection<ConnectionEnd::SERVER>> connections;
    GameStore *store;

    static inline std::unique_ptr <std::thread> thread_{nullptr};

public:
    GameServer()
        : store{GameStore::getInstance()} {
        this->initWorld();
    }

    void tick() {
        std::lock_guard <std::mutex> lock(this->store->worldMutex);

        const auto timeSpan = getTimeSpan("server");
        auto world = this->store->getWorld();

        // snakes might be deleted during iteration
        // make copy of the original map
        auto snakes = world->snakes;
        for (auto &[snakeId, snake]: snakes) {
            if (snake->isDying) {
                if (snake->deadTicks < -100) {
                    snake->deadTicks = 25;
                } else {
                    snake->deadTicks--;
                }

                if (snake->deadTicks == 0) {
                    snake->turnIntoFood();
                } else if (snake->deadTicks < 0) {
                    if (snake->isBot || snake->deadTicks < -100) {
                        world->removeSnake(snake);
                    }
                }

                continue;
            }

            snake->move(timeSpan);
            snake->updateStatus();
            snake->checkFoodEaten();
        }

        /**
         * collision detect
         */
        for (auto &[thisSnakeId, thisSnake]: world->snakes) {
            if (thisSnake->isDying) {
                continue;
            }

            auto thisHead = thisSnake->getHeadTipBoundBox();

            // detect collision with border
            if (!world->isIntersect(thisHead)) {
                thisSnake->isDying = true;
                continue;
            }

            // detect collision with other snakes
            for (auto &[otherSnakeId, otherSnake]: world->snakes) {
                if (thisSnakeId == otherSnakeId ||
                    otherSnake->isDying ||
                    !thisSnake->zone.isIntersect(otherSnake->zone)) {
                    continue;
                }

                // case 1. head to head (tip)
                if (thisHead.isIntersect(otherSnake->getHeadTipBoundBox())) {
                    thisSnake->isDying = true;
                    otherSnake->isDying = true;
                    continue;
                }

                // case 2. head to head (not tip)
                if (thisHead.isIntersect(otherSnake->head.getBoundBox(otherSnake))) {
                    thisSnake->isDying = true;
                    continue;
                }

                // case 3. head to body
                for (auto &bodyPart: otherSnake->bodyParts) {
                    if (thisHead.isIntersect(bodyPart.getBoundBox(otherSnake))) {
                        thisSnake->isDying = true;
                        continue;
                    }
                }
            }
        }

        if (getFrameCount("server") % 100 == 0) {
            // every 100 ticks
            // fill foods and bot snakes
            for (auto &sector: world->sectors) {
                sector.fillFood(true);
            }

            world->fillSnake();
        }
    }

    void notify() {
        // Not implemented yet
    }

    void loop() {
        while (true) {
            sleepFor(125, "server"); // tick time 8ms (125tps)

            this->tick();
            this->notify();
        }
    }

    static void start() {
        if (!isRunning()) {
            thread_ = std::make_unique<std::thread>([]() {
                auto server = GameServer();

                server.store->worldInitialized.notify_all();

                server.loop();
            });
        }
    }

    static bool isRunning() {
        return thread_ != nullptr;
    }

    static void stop() {
        if (thread_ != nullptr) {
            thread_.reset();
        }
    }

private:
    void initWorld() {
        const GameConfig config{};

        std::lock_guard <std::mutex> lock(this->store->worldMutex);

        this->store->createWorld(config);
        auto *world = this->store->getWorld();

#ifdef EASYSLITHER_CLIENT // if is single-player
        // add current player

        auto &username = this->store->username;
        auto *snake = world->createSnake(false, username, true);

        world->addSnake(snake);
#endif //EASYSLITHER_CLIENT
    }
};
