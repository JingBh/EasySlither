export module Internal.GameStore;

#ifdef DEBUG

import <cmath>;
import <cstdint>;
import <numbers>;

#endif //DEBUG

import <memory>;
import <mutex>;

import Game;
import Game.Config;
import Network.ConnectionEnd;

export template<ConnectionEnd E>
class GameStore {
private:
    std::unique_ptr <World> world;

    static inline GameStore<E> *instance_{nullptr};
    static inline std::mutex mutex_;

public:
    void createWorld(const GameConfig config) {
        this->world = std::make_unique<World>(config);
    }

    auto getWorld() {
        return this->world.get();
    }

#ifdef DEBUG

    void generateTestData() {
        const auto config = GameConfig{};
        this->createWorld(config);

        // add foods
        for (int i = 0; i < 360; i += 10) {
            auto *food = new Food(
                config,
                static_cast<int16_t>(100 * std::cos(std::numbers::pi * i / 180)),
                static_cast<int16_t>(100 * std::sin(std::numbers::pi * i / 180)),
                20);
            this->world->addFood(food);
        }

        // add player snake
        auto *player = new Snake(0, 0, 0, false, true, "Player");
        this->world->addSnake(player);
    }

#endif //DEBUG

    static GameStore<E> *getInstance() {
        mutex_.lock();
        if (instance_ == nullptr) {
            instance_ = new GameStore<E>();
        }
        mutex_.unlock();

        return instance_;
    }
};
