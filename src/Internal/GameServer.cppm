export module Internal.GameServer;

import <atomic>;
import <cstdint>;
import <cstdio>;
import <list>;
import <memory>;
import <mutex>;
import <string>;
import <thread>;

import Game;
import Game.GameConfig;
import Internal.GameStore;
import Network.Connection;
import Utils.Frames;

export class GameServer {
private:
    std::list <IConnection<ConnectionEnd::SERVER>> connections;
    GameStore *store;

    static inline std::unique_ptr <std::thread> thread_{nullptr};
    static inline std::atomic_bool stop_{false};

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
                snake->deadTicks--;

                if (snake->deadTicks == 0) {
                    if (snake->isPlayer) {
                        std::lock_guard <std::mutex> rankLock(this->store->rankMutex);
                        this->store->lastGameScore = snake->getScore();
                        this->store->updateRank(snake->username, this->store->lastGameScore);
                    }

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
                thisSnake->setDying();
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
                    thisSnake->setDying();
                    otherSnake->setDying();
                    continue;
                }

                // case 2. head to head (not tip)
                if (thisHead.isIntersect(otherSnake->head.getBoundBox(otherSnake))) {
                    thisSnake->setDying();
                    continue;
                }

                // case 3. head to body
                for (auto &bodyPart: otherSnake->bodyParts) {
                    if (thisHead.isIntersect(bodyPart.getBoundBox(otherSnake))) {
                        thisSnake->setDying();
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
            if (stop_) {
                return;
            }

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
        if (isRunning()) {
            stop_ = true;
            thread_->join();
            thread_.reset();
            stop_ = false;
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

        auto username = this->store->getUsername();
        auto *snake = world->createSnake(false, username, true);

        world->addSnake(snake);
#endif //EASYSLITHER_CLIENT
    }
};
