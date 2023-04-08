export module Utils.Mediator;

import <optional>;

export template<typename T>
class IMediator {
protected:
    std::optional <T> pendingChange;

public:
    IMediator(IMediator<T> &other) = delete;

    virtual void operator=(const IMediator<T> &) = delete;

    virtual void notify(const T &event) {
        this->pendingChange = event;
    }

    virtual void applyChanges() {
        this->pendingChange.reset();
    }

protected:
    IMediator() = default;

    virtual ~IMediator() = default;
};
