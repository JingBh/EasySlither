export module Utils.Observer;

import <list>;
import <string>;

export template<typename T>
class IObserver {
public:
    virtual ~IObserver() = default;

    virtual void update(const T &message) = 0;
};

export template<typename T>
class ISubject {
protected:
    std::list<IObserver<T> *> observers;

protected:
    ISubject() = default;

    virtual ~ISubject() = default;

public:
    ISubject(ISubject<T> &other) = delete;

    virtual void operator=(const ISubject<T> &) = delete;

    void attach(IObserver<T> *observer) {
        observers.push_back(observer);
    }

    void detach(IObserver<T> *observer) {
        observers.remove(observer);
    }

    void notify(const T &message) {
        for (auto observer: observers) {
            observer->update(message);
        }
    }
};
