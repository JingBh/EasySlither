export module Internal.GameStore;

import <algorithm>;
import <condition_variable>;
import <cstdint>;
import <memory>;
import <mutex>;
import <optional>;
import <string>;
import <tuple>;
import <utility>;
import <vector>;

import Game;
import Game.GameConfig;
import Game.GameType;
import Utils.Filesystem;

export class GameStore {
public:
    std::mutex worldMutex;
    std::mutex rankMutex;

    std::condition_variable worldInitialized;
    std::mutex worldInitializationMutex;

    uint32_t lastGameScore = 0;
    std::optional <GameType> lastGameType;

private:
    std::string username;
    std::string serverAddress;

    std::vector <std::tuple<std::string, uint16_t>> rank;
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

    auto *getRank() {
        if (this->rank.empty()) {
            auto file = openPersistFile("rank.txt", std::ios::in);

            std::string name;
            uint32_t score;
            while (file >> name >> score) {
                this->rank.emplace_back(name, score);
            }

            std::sort(this->rank.begin(), this->rank.end(), [](const auto &a, const auto &b) {
                return std::get<1>(a) > std::get<1>(b);
            });
        }

        return &this->rank;
    }

    void updateRank(const std::string thisUsername, const uint32_t score) {
        this->getRank();

        auto it = std::find_if(this->rank.begin(), this->rank.end(), [&thisUsername](const auto &item) {
            return std::get<0>(item) == thisUsername;
        });

        if (it == this->rank.end()) {
            this->rank.emplace_back(thisUsername, score);
        } else {
            std::get<1>(*it) = score;
        }

        std::sort(this->rank.begin(), this->rank.end(), [](const auto &a, const auto &b) {
            return std::get<1>(a) > std::get<1>(b);
        });

        auto file = openPersistFile("rank.txt", std::ios::out);
        for (const auto &item: this->rank) {
            file << std::get<0>(item) << ' ' << std::get<1>(item) << '\n';
        }
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
