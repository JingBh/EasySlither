export module Network.Connection;

import <cstdint>;
import <vector>;

import Internal.GameStore;
export import Network.ConnectionEnd;
export import Network.ConnectionType;
import Network.ITCQueue;

export template<ConnectionEnd E>
class IConnection;

export template<ConnectionEnd E>
class LocalConnection;

template<ConnectionEnd E>
class IConnection {
protected:
    GameStore <E> *store = GameStore<E>::getInstance();

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

template<ConnectionEnd E>
class LocalConnection : public IConnection<E> {
public:
    LocalConnection() = default;
};

template<>
class LocalConnection<ConnectionEnd::CLIENT> {
protected:
    ITCQueue <ConnectionEnd::CLIENT> *myQueue = ITCQueue<ConnectionEnd::CLIENT>::getInstance();
    ITCQueue <ConnectionEnd::SERVER> *targetQueue = ITCQueue<ConnectionEnd::SERVER>::getInstance();

public:

};

template<>
class LocalConnection<ConnectionEnd::SERVER> {
protected:
    ITCQueue <ConnectionEnd::SERVER> *myQueue = ITCQueue<ConnectionEnd::SERVER>::getInstance();
    ITCQueue <ConnectionEnd::CLIENT> *targetQueue = ITCQueue<ConnectionEnd::CLIENT>::getInstance();

public:

};
