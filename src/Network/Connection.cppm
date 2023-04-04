export module Network.Connection;

import <cstdint>;
import <vector>;

import Internal.GameStore;
export import Network.ConnectionEnd;
export import Network.ConnectionType;

export template<ConnectionEnd E>
class IConnection;

template<ConnectionEnd E>
class IConnection {
protected:
    GameStore *store = GameStore::getInstance();

protected:
    IConnection() = default;

    virtual ~IConnection() = default;

    virtual void send(std::vector <uint8_t>) = 0;

    virtual void receive() = 0;

    virtual void onReceived(std::vector <uint8_t>) = 0;
};

template<>
class IConnection<ConnectionEnd::CLIENT> {
public:

};

template<>
class IConnection<ConnectionEnd::SERVER> {
public:

};
