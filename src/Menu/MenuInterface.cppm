/**
 * Interface of the Menu module.
 */

export module Menu;

import <memory>;
import <string>;
import <variant>;
import <vector>;

import Internal.UserInput;
import Screen.ScreenName;
import ThirdParty;
import Utils.Drawable;
import Utils.Mediator;

export class Menu;

export class MenuItem;

export class TextInput;

export using IMenuItem = std::variant<MenuItem, TextInput>;

class Menu : public Drawable,
             public ObservesKeyPress, public ObservesMouseMove {
protected:
    std::vector <IMenuItem> items;
    std::size_t activeItem = -1;

    int itemWidth = 192;
    int itemHeight = 48;
    int itemGap = 24;

    void onKeyPress(const KeyType key) override;

    void onMouseMove(const windows::Point &point) override;

public:
    ~Menu() override {
        ObservesKeyPress::~ObservesKeyPress();
        ObservesMouseMove::~ObservesMouseMove();
    }

    [[nodiscard]] std::unique_ptr <easyx::Image> renderImage() const override;

    void emplaceItem(IMediator <ScreenName> *mediator, const ScreenName event, const std::string text);

    void emplaceItem(std::string *value, const size_t maxLength = 32);
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

class TextInput : ObservesTextInput {
public:
    std::string *value;

private:
    size_t maxLength;

public:
    TextInput(std::string *value, const size_t maxLength = 32)
        : value{value}, maxLength{maxLength}, ObservesTextInput() {}

protected:
    void onTextInput(char character) override;
};
