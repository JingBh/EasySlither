/**
 * Interface of the Menu module.
 */

export module Menu;

import <memory>;
import <string>;
import <vector>;

import Middleware.UserInput;
import Screen.ScreenName;
import ThirdParty;
import Utils.Drawable;
import Utils.Mediator;

export class Menu;

export class MenuItem;

class Menu : public Drawable, public ObservesKeyPress, public ObservesMouseMove {
protected:
    std::vector <MenuItem> items;
    std::size_t activeItem = -1;

    int itemWidth = 192;
    int itemHeight = 48;
    int itemGap = 32;

    void onKeyPress(const KeyType key) override;

    void onMouseMove(const windows::Point &point) override;

public:
    [[nodiscard]] std::unique_ptr <easyx::Image> renderImage() const override;

    void addItem(MenuItem &item);

    void emplaceItem(IMediator <ScreenName> *mediator, const ScreenName event, const std::string text);
};

class MenuItem : Mediatable<ScreenName> {
public:
    const std::string text;

protected:
    const ScreenName event;

public:
    MenuItem(IMediator <ScreenName> *mediator, const ScreenName event, const std::string text)
        : event{event}, text{text}, Mediatable(mediator) {}

    void action() const;
};
