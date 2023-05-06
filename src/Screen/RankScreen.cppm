export module Screen.RankScreen;

import <algorithm>;
import <cstdint>;
import <memory>;
import <mutex>;
import <string>;
import <tuple>;

import Internal.GameStore;
import Menu;
import Screen;
import Screen.ScreenName;
import ThirdParty;
import Utils.Colors;
import Utils.Drawable;
import Utils.TextEncode;

RankScreen::RankScreen(Screen &screen)
    : screen{screen}, store{GameStore::getInstance()} {
    this->menu = std::make_unique<Menu>();
    this->menu->emplaceItem(ScreenName::MAIN, "返回");
}

[[nodiscard]] std::unique_ptr <easyx::Image> RankScreen::renderImage() const {
    auto image = std::make_unique<easyx::Image>(
        this->screen.getWidth(), this->screen.getHeight());
    const auto width = image->getwidth();
    const auto height = image->getheight();

    const auto titleFontSize = 48;
    const auto titleMargin = 32;
    const auto fontSize = 20;
    const auto margin = 8;
    const auto numberWidth = 20;
    const auto scoreWidth = 64;
    const auto estimateHeight = titleFontSize + titleMargin + // title
                                margin * 22 + fontSize * 11 + // rank
                                titleMargin + 52; // menu
    const auto startHeight = (height - estimateHeight) / 2;
    const auto endHeight = height - startHeight;

    easyx::setWorkingImage(image.get());
    easyx::setBackgroundColor(GRAY_900);
    easyx::setTextColor(GRAY_100);
    easyx::clearDevice();

    {
        windows::Rect location{0,
                               startHeight,
                               width,
                               height};
        easyx::setFont(titleFontSize, true);
        easyx::setTextColor(WHITE);
        easyx::drawText(encode("排行榜"), &location,
                        easyx::TEXT_CENTER | easyx::TEXT_SINGLELINE);
    }

    {
        std::string username;
        uint32_t score;
        bool isSelfShown = false;
        bool isSelf = false;

        std::lock_guard <std::mutex> lock{this->store->rankMutex};
        const auto rank = this->store->getRank();

        windows::Rect location{width / 2 - 160,
                               startHeight + titleFontSize + titleMargin,
                               width / 2 + 160,
                               startHeight + titleFontSize + titleMargin + fontSize + margin * 2};

        easyx::setLineStyle(1);
        easyx::setLineColor(GRAY_500);

        for (size_t i = 1; i <= 11 && i < rank->size(); i++) {
            if (i == 11) { // top 10 already ended
                if (isSelfShown) {
                    break;
                }

                // self not in top 10, find it and show in the bottom
                for (size_t j = 10; j < rank->size(); j++) {
                    const auto &[thisUsername, thisScore] = rank->at(j);

                    if (thisUsername == this->store->getUsername()) {
                        i = j + 1;
                        username = thisUsername;
                        score = thisScore;
                        isSelf = true;
                        break;
                    }
                }

                if (!isSelf) {
                    break;
                }
            } else {
                std::tie(username, score) = rank->at(i - 1);
                isSelf = username == this->store->getUsername();
            }

            if (isSelf) {
                isSelfShown = true;
            }

            easyx::drawLine(location.left, location.top,
                            location.right, location.top);

            windows::Rect numberLocation{location.left + margin,
                                         location.top + margin,
                                         location.left + margin + numberWidth,
                                         location.bottom - margin};
            easyx::setFont(fontSize, true);
            easyx::setTextColor(isSelf ? AMBER_500 : BLUE_500);
            easyx::drawText(std::to_string(i), &numberLocation,
                            easyx::TEXT_CENTER | easyx::TEXT_SINGLELINE);

            windows::Rect usernameLocation{location.left + numberWidth + margin * 2,
                                           location.top + margin,
                                           location.right - margin * 2 - scoreWidth,
                                           location.bottom - margin};
            easyx::setFont(fontSize);
            easyx::setTextColor(isSelf ? AMBER_500 : GRAY_300);
            easyx::drawText(encode(username), &usernameLocation,
                            easyx::TEXT_SINGLELINE | easyx::TEXT_WORD_ELLIPSIS);

            windows::Rect scoreLocation{location.right - margin - scoreWidth,
                                        location.top + margin,
                                        location.right - margin,
                                        location.bottom - margin};
            easyx::setTextColor(GRAY_100);
            easyx::drawText(std::to_string(score), &scoreLocation,
                            easyx::TEXT_RIGHT | easyx::TEXT_SINGLELINE | easyx::TEXT_NOCLIP);

            easyx::drawLine(location.left, location.bottom,
                            location.right, location.bottom);

            location.top = location.bottom;
            location.bottom += fontSize + margin * 2;
        }
    }

    {
        windows::Point location{width / 2,
                                endHeight - 52};
        this->menu->renderAt(location, image.get(), RenderAnchor::TOP_MIDDLE);
    }

    easyx::setWorkingImage();

    return image;
}
