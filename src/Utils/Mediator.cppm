export module Utils.Mediator;

export template<typename T>
class IMediator {
protected:
    IMediator() = default;

    virtual ~IMediator() = default;

public:
    IMediator(IMediator<T> &other) = delete;

    virtual void operator=(const IMediator<T> &) = delete;

    virtual void notify(const T &event) = 0;
};

export template<typename T>
class Mediatable {
protected:
    virtual ~Mediatable() = default;

    IMediator<T> *_mediator;

public:
    explicit Mediatable(IMediator<T> *mediator = nullptr)
        : _mediator(mediator) {}
};
