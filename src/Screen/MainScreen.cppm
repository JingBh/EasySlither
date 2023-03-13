export module Screen.MainScreen;

import <memory>;

import Menu;
import Middleware.ScreenMediator;
import Screen;
import ThirdParty;
import Utils.Drawable;
import Utils.TextEncode;

[[nodiscard]] std::unique_ptr <easyx::Image> MainScreen::renderImage() const {
    auto image = std::make_unique<easyx::Image>(
        this->screen.getWidth(), this->screen.getHeight());

    easyx::setWorkingImage(image.get());

    this->renderTitle(image.get());
    this->renderSign(image.get());

    easyx::setWorkingImage();

    return image;
}

void MainScreen::renderTitle(easyx::Image *image) const {
    windows::Rect location{0,
                           image->getheight() / 7,
                           image->getwidth(),
                           image->getheight()};
    easyx::setFont(64, true);
    easyx::drawText("EasySlither", &location,
                    easyx::TEXT_CENTER | easyx::TEXT_SINGLELINE);
}

void MainScreen::renderSign(easyx::Image *image) const {
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

void MainScreen::render() {
    windows::Point renderLocation{0, 0};
    this->renderAt(renderLocation);
}

class MainMenu : public Menu {
    MainMenu() {
        auto mediator = ScreenMediator::getInstance();

        this->emplaceItem(mediator, ScreenName::SINGLE_PLAYER, "单人游戏");
        this->emplaceItem(mediator, ScreenName::MULTI_PLAYER, "多人游戏");
    }
};
