export module Utils.RankScreen;

import <cstdint>;
import <memory>;

import Internal.GameStore;
import Menu;
import Screen;
import Screen.ScreenName;
import Utils.Colors;
import Utils.Drawable;
import Utils.TextEncode;

RankScreen::RankScreen(Screen &screen)
    : screen{screen}, store{GameStore::getInstance()} {
    this->menu = std::make_unique<Menu>();
    this->menu->emplaceItem(ScreenName::MAIN, "返回主菜单");
}

[[nodiscard]] std::unique_ptr <easyx::Image> RankScreen::renderImage() const {
    auto image = std::make_unique<easyx::Image>(
        this->screen.getWidth(), this->screen.getHeight());
    const auto width = image->getwidth();
    const auto height = image->getheight();

    easyx::setWorkingImage(image.get());
    easyx::setBackgroundColor(GRAY_900);
    easyx::setTextColor(GRAY_100);
    easyx::clearDevice();

    easyx::setWorkingImage();

    return image;
}
