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

        for (auto &[snakeId, snake]: world->snakes) {
            snake->move(timeSpan);
            snake->updateStatus();
            snake->checkFoodEaten();
        }

        for (auto &[snakeId, snake]: world->snakes) {
            // TODO: check snake collision
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
