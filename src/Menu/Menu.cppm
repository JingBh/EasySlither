export module Menu.Menu;

import <memory>;
import <variant>;

import Menu;
import Internal.UserInput;
import Screen.ScreenName;
import ThirdParty;
import Utils.Colors;
import Utils.Drawable;
import Utils.Mediator;
import Utils.TextEncode;

std::unique_ptr <easyx::Image> Menu::renderImage() const {
    auto image = std::make_unique<easyx::Image>(
        itemWidth + 4,
        (itemHeight + itemGap) * static_cast<int>(items.size()) - itemGap + 4);

    easyx::setWorkingImage(image.get());
    easyx::setBackgroundColor(GRAY_900);
    easyx::setTextColor(WHITE);
    easyx::clearDevice();
    easyx::setFont(24, true);

    for (std::size_t i = 0; i < items.size(); i++) {
        const auto item = items[i];
        const bool isButton = std::holds_alternative<MenuItem>(item);

        if (activeItem == i) {
            easyx::setLineStyle(2); // +4 in size and +2 in position are the margins for the border here
            easyx::setLineColor(isButton ? GRAY_100 : GREEN_700);
            easyx::setFillColor(isButton ? GREEN_700 : ZINC_700);
        } else {
            easyx::setFillColor(isButton ? GREEN_600 : ZINC_700);
        }

        windows::Rect location{
            2,
            static_cast<int>((itemHeight + itemGap) * i) + 1,
            itemWidth + 2,
            static_cast<int>(itemHeight * (i + 1) + itemGap * i)
        };
        easyx::drawRoundRect(location, itemHeight / 2, activeItem == i);

        if (activeItem == i) {
            easyx::setBackgroundColor(isButton ? GREEN_700 : ZINC_700);
        } else {
            easyx::setBackgroundColor(isButton ? GREEN_600 : ZINC_700);
        }

        if (std::holds_alternative<MenuItem>(item)) {
            easyx::drawText(encode(std::get<MenuItem>(item).text), &location,
                            easyx::TEXT_CENTER | easyx::TEXT_VCENTER | easyx::TEXT_SINGLELINE);
        } else if (std::holds_alternative<TextInput>(item)) {
            easyx::drawText(encode(*std::get<TextInput>(item).value), &location,
                            easyx::TEXT_CENTER | easyx::TEXT_VCENTER | easyx::TEXT_SINGLELINE |
                            easyx::TEXT_EDITCONTROL);
        }

        easyx::setBackgroundColor(GRAY_900);
    }

    easyx::setWorkingImage();

    return image;
}

void Menu::emplaceItem(IMediator <ScreenName> *mediator, const ScreenName event, const std::string text) {
    const IMenuItem item{std::in_place_type < MenuItem > , mediator, event, text};
    items.push_back(item);
}

void Menu::emplaceItem(std::string *value, const size_t maxLength) {
    const IMenuItem item{std::in_place_type < TextInput > , value, maxLength};
    items.push_back(item);
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
                const auto item = items[activeItem];
                if (std::holds_alternative<MenuItem>(item)) {
                    std::get<MenuItem>(item).action();
                }
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
