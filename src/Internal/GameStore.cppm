export module Internal.GameStore;

import <condition_variable>;
import <memory>;
import <mutex>;
import <string>;

import Game;
import Game.Config;

export class GameStore {
public:
    std::string username = "玩家";
    std::mutex worldMutex;

    std::condition_variable worldInitialized;
    std::mutex worldInitializationMutex;

private:
    std::unique_ptr <World> world;

    static inline GameStore *instance_{nullptr};
    static inline std::mutex mutex_;

public:
    void createWorld(const GameConfig config) {
        this->world = std::make_unique<World>(config);
    }

    auto getWorld() {
        return this->world.get();
    }

    static GameStore *getInstance() {
        mutex_.lock();
        if (instance_ == nullptr) {
            instance_ = new GameStore();
        }
        mutex_.unlock();

        return instance_;
    }
};
