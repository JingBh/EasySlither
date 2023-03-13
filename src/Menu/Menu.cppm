export module Menu;

import <memory>;
import <vector>;

import Menu.MenuItem;
import Middleware.UserInput;
import Screen;
import ThirdParty;
import Utils.Drawable;
import Utils.Mediator;

export class Menu
    : public Drawable, public ObservesInputDirection, public ObservesMouseMove {
protected:
    std::vector <MenuItem> items;
    std::size_t activeItem = -1;

    int itemWidth = 96;
    int itemHeight = 32;
    int itemGap = 16;

    void addItem(MenuItem &item) {
        items.push_back(item);
    }

    void emplaceItem(IMediator <ScreenName> *mediator, const ScreenName event, const std::string text) {
        items.emplace_back(mediator, event, text);
    }

public:
    [[nodiscard]] std::unique_ptr <easyx::Image> renderImage() const override {
        auto image = std::make_unique<easyx::Image>(
            itemWidth, itemHeight * static_cast<int>(items.size()) +
                       itemGap * static_cast<int>(items.size() - 1));

        for (std::size_t i = 0; i < items.size(); i++) {
            const auto item = items[i];


        }

        return image;
    }

    void onInputDirection(const int degree) override {
        if (degree == 0) {
            if (activeItem > 0) {
                activeItem--;
            } else {
                activeItem = items.size() - 1;
            }
        } else if (degree == 180) {
            if (activeItem < items.size() - 1) {
                activeItem++;
            } else {
                activeItem = 0;
            }
        }
    }

    void onMouseMove(const windows::Point &point) override {

    }
};
