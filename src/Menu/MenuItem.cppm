export module Menu.MenuItem;

import <string>;

import Middleware.ScreenMediator;
import Screen;
import Utils.Drawable;
import Utils.Mediator;

export class MenuItem : Mediatable<ScreenName> {
public:
    const std::string text;

protected:
    const ScreenName event;

public:
    MenuItem(IMediator <ScreenName> *mediator, const ScreenName event, const std::string text)
        : event{event}, text{text}, Mediatable(mediator) {}

    void action() const {
        this->_mediator->notify(this->event);
    }
};
