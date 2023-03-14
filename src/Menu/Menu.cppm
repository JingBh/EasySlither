export module Menu.Menu;

import <memory>;

import Menu;
import Middleware.UserInput;
import Screen.ScreenName;
import ThirdParty;
import Utils.Colors;
import Utils.Mediator;
import Utils.TextEncode;
import Utils.Throttle;

std::unique_ptr <easyx::Image> Menu::renderImage() const {
    auto image = std::make_unique<easyx::Image>(
        itemWidth,
        (itemHeight + itemGap) * static_cast<int>(items.size()) - itemGap);

    easyx::setWorkingImage(image.get());
    easyx::setBackgroundColor(GRAY_900);
    easyx::clearDevice();
    easyx::setFont(24, true);

    for (std::size_t i = 0; i < items.size(); i++) {
        const auto item = items[i];

        if (activeItem == i) {
            easyx::setFillColor(GREEN_700);
        } else {
            easyx::setFillColor(GREEN_600);
        }

        windows::Rect location{
            0,
            static_cast<int>((itemHeight + itemGap) * i),
            itemWidth,
            static_cast<int>(itemHeight * (i + 1) + itemGap * i)
        };
        easyx::drawRoundRect(location, itemHeight / 2, false);

        if (activeItem == i) {
            easyx::setBackgroundColor(GREEN_700);
        } else {
            easyx::setBackgroundColor(GREEN_600);
        }

        easyx::drawText(encode(item.text), &location,
                        easyx::TEXT_CENTER | easyx::TEXT_VCENTER | easyx::TEXT_SINGLELINE);

        easyx::setBackgroundColor(GRAY_900);
    }

    easyx::setWorkingImage();

    return image;
}

void Menu::addItem(MenuItem &item) {
    items.push_back(item);
}

void Menu::emplaceItem(IMediator <ScreenName> *mediator, const ScreenName event, const std::string text) {
    items.emplace_back(mediator, event, text);
}

void Menu::onKeyPress(const KeyType key) {
    switch (key) {
        case KeyType::UP:
            if (activeItem > 0) {
                activeItem--;
            } else {
                activeItem = items.size() - 1;
            }
            break;
        case KeyType::DOWN:
        case KeyType::SWITCH:
            if (activeItem < items.size() - 1) {
                activeItem++;
            } else {
                activeItem = 0;
            }
            break;
        case KeyType::CONFIRM:
            if (activeItem >= 0 && activeItem < items.size()) {
                items[activeItem].action();
            }
            break;
        default:
            break;
    }
}

void Menu::onMouseMove(const windows::Point &point) {
    if (this->renderRect == nullptr) {
        return;
    }

    for (std::size_t i = 0; i < items.size(); i++) {
        windows::Rect itemRenderRect{*this->renderRect};

        itemRenderRect.top += static_cast<int>((itemHeight + itemGap) * i);
        itemRenderRect.bottom = itemRenderRect.top + static_cast<int>(itemHeight);

        if (windows::isInRect(itemRenderRect, point)) {
            activeItem = i;
            return;
        }
    }

    activeItem = -1;
}
