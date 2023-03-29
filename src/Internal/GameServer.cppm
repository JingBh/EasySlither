export module Internal.GameServer;

import <cstdint>;
import <list>;

import Game;
import Game.Config;
import Internal.GameStore;
import Network.Connection;

export class GameServer {
private:
    std::list <IConnection<ConnectionEnd::SERVER>> connections;
    GameStore <ConnectionEnd::SERVER> *store;
    int32_t nextSnakeId = 0; // auto-increment id

public:
    GameServer()
        : store{GameStore<ConnectionEnd::SERVER>::getInstance()} {
        this->initWorld();
    }

    /* ~GameServer() {
        this->destroyWorld();
    } */

    void tick() {

    }

private:
    void initWorld() {
        const GameConfig config{};
        this->store->createWorld(config);

        this->fillSnakes();
    }

    /**
     * Fill the world with bot snakes if there are not enough players
     */
    void fillSnakes() {

    }
};
