export module Screen.GameOverScreen;

import <cstdint>;
import <memory>;

import Game.GameType;
import Internal.GameStore;
import Menu;
import Screen;
import Screen.ScreenName;
import ThirdParty;
import Utils.Colors;
import Utils.Drawable;
import Utils.TextEncode;

GameOverScreen::GameOverScreen(Screen &screen)
    : screen{screen}, store{GameStore::getInstance()} {
    this->menu = std::make_unique<Menu>();

    switch (this->store->lastGameType.value()) {
        using
        enum GameType;
        case SINGLE_PLAYER:
            this->menu->emplaceItem(ScreenName::SINGLE_PLAYER_START, "再来一次");
            this->menu->emplaceItem(ScreenName::SINGLE_PLAYER_RANK, "排行榜");
            break;
        case MULTI_PLAYER:
            this->menu->emplaceItem(ScreenName::MULTI_PLAYER_START, "再来一次");
            break;
    }

    this->menu->emplaceItem(ScreenName::MAIN, "返回主菜单");
}

[[nodiscard]] std::unique_ptr <easyx::Image> GameOverScreen::renderImage() const {
    auto image = std::make_unique<easyx::Image>(
        this->screen.getWidth(), this->screen.getHeight());
    const auto width = image->getwidth();
    const auto height = image->getheight();

    easyx::setWorkingImage(image.get());
    easyx::setBackgroundColor(GRAY_900);
    easyx::setTextColor(GRAY_100);
    easyx::clearDevice();

    {
        windows::Rect location{0,
                               height / 7,
                               width,
                               height};
        easyx::setFont(64, true);
        easyx::setTextColor(GREEN_500);
        easyx::drawText(encode("游戏结束"), &location,
                        easyx::TEXT_CENTER | easyx::TEXT_SINGLELINE);
    }

    {
        const auto hintText = this->store->getUsername() +
                              "，你的得分是 " +
                              std::to_string(this->store->lastGameScore);

        windows::Rect location{0,
                               (height / 7 + 64 + height / 3) / 2 - 7,
                               width,
                               height};
        easyx::setFont(14);
        easyx::setTextColor(GRAY_300);
        easyx::drawText(encode(hintText), &location,
                        easyx::TEXT_CENTER | easyx::TEXT_SINGLELINE);
    }

    {
        windows::Point location{width / 2,
                                height / 3};
        this->menu->renderAt(location, image.get(), RenderAnchor::TOP_MIDDLE);
    }

    easyx::setWorkingImage();

    return image;
}
