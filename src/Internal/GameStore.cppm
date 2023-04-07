export module Internal.GameStore;

import <condition_variable>;
import <memory>;
import <mutex>;
import <string>;
import <utility>;

import Game;
import Game.Config;
import Utils.Filesystem;

export class GameStore {
public:
    std::mutex worldMutex;

    std::condition_variable worldInitialized;
    std::mutex worldInitializationMutex;

private:
    std::string username;
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

    std::string getUsername() {
        if (this->username.empty()) {
            auto file = openPersistFile("username.txt", std::ios::in);
            file >> this->username;
        }

        if (this->username.empty()) {
            this->username = "玩家";
        }

        return this->username;
    }

    void setUsername(std::string_view newUsername) {
        auto file = openPersistFile("username.txt", std::ios::out);
        this->username = newUsername;
        file << this->username;
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
