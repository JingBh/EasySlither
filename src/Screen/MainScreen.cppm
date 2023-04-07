export module Screen.MainScreen;

import <memory>;

import Menu;
import Internal.GameStore;
import Internal.ScreenMediator;
import Screen;
import Screen.ScreenName;
import ThirdParty;
import Utils.Colors;
import Utils.Drawable;
import Utils.TextEncode;

void renderTitle(easyx::Image *image);

void renderMenu(easyx::Image *image, Menu *menu);

void renderSignature(easyx::Image *image);

MainScreen::MainScreen(Screen &screen)
    : screen{screen} {
    auto mediator = ScreenMediator::getInstance(&screen);

    this->menu = std::make_unique<Menu>();
    this->menu->emplaceItem(&GameStore::getInstance()->username, 32);
    this->menu->emplaceItem(mediator, ScreenName::SINGLE_PLAYER, "单人游戏");
    this->menu->emplaceItem(mediator, ScreenName::MULTI_PLAYER, "多人游戏");
    this->menu->emplaceItem(mediator, ScreenName::EXIT, "退出游戏");
}

[[nodiscard]] std::unique_ptr <easyx::Image> MainScreen::renderImage() const {
    auto image = std::make_unique<easyx::Image>(
        this->screen.getWidth(), this->screen.getHeight());

    easyx::setWorkingImage(image.get());
    easyx::setBackgroundColor(GRAY_900);
    easyx::setTextColor(GRAY_100);
    easyx::clearDevice();

    renderTitle(image.get());
    renderMenu(image.get(), this->menu.get());
    renderSignature(image.get());

    easyx::setWorkingImage();

    return image;
}

void renderTitle(easyx::Image *image) {
    windows::Rect location{0,
                           image->getheight() / 7,
                           image->getwidth(),
                           image->getheight()};
    easyx::setFont(64, true);
    easyx::drawText("EasySlither", &location,
                    easyx::TEXT_CENTER | easyx::TEXT_SINGLELINE);
}

void renderMenu(easyx::Image *image, Menu *menu) {
    windows::Point location{image->getwidth() / 2,
                            image->getheight() / 3};

    menu->renderAt(location, image, RenderAnchor::TOP_MIDDLE);
}

void renderSignature(easyx::Image *image) {
    windows::Rect location{8,
                           image->getheight() - 24,
                           image->getwidth(),
                           image->getheight() - 8};
    easyx::setFont(16);
    easyx::drawText(encode("北京工业大学\t高级语言程序设计课设"),
                    &location,
                    easyx::TEXT_LEFT | easyx::TEXT_VCENTER | easyx::TEXT_EXPANDTABS);

    location = {0,
                image->getheight() - 24,
                image->getwidth() - 8,
                image->getheight() - 8};
    easyx::drawText(encode("22080206\t敬博浩"),
                    &location,
                    easyx::TEXT_RIGHT | easyx::TEXT_VCENTER | easyx::TEXT_EXPANDTABS);
}
