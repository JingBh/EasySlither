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

MainScreen::MainScreen(Screen &screen)
    : screen{screen} {
    auto mediator = ScreenMediator::getInstance(&screen);

    this->menu = std::make_unique<Menu>();
    this->menu->emplaceItem(mediator, ScreenName::USERNAME_INPUT, "修改昵称");
    this->menu->emplaceItem(mediator, ScreenName::SINGLE_PLAYER_MAIN, "单人游戏");
    this->menu->emplaceItem(mediator, ScreenName::MULTI_PLAYER_MAIN, "多人游戏");
    this->menu->emplaceItem(mediator, ScreenName::EXIT, "退出");
}

MainScreenSinglePlayer::MainScreenSinglePlayer(Screen &screen)
    : MainScreen(screen, std::make_unique<Menu>()) {
    auto mediator = ScreenMediator::getInstance(&screen);

    this->menu->emplaceItem(mediator, ScreenName::MAIN, "返回主菜单");
    this->menu->emplaceItem(mediator, ScreenName::SINGLE_PLAYER_START, "开始游戏");
    this->menu->emplaceItem(mediator, ScreenName::SINGLE_PLAYER_RANK, "排行榜");
}

MainScreenMultiPlayer::MainScreenMultiPlayer(Screen &screen)
    : MainScreen(screen, std::make_unique<Menu>()) {
    auto mediator = ScreenMediator::getInstance(&screen);

    this->menu->emplaceItem(mediator, ScreenName::MAIN, "返回主菜单");
    this->menu->emplaceItem(mediator, ScreenName::MULTI_PLAYER_START, "开始游戏");
    this->menu->emplaceItem(mediator, ScreenName::SERVER_INPUT, "选择服务器");
}

[[nodiscard]] std::unique_ptr <easyx::Image> MainScreen::renderImage() const {
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
        easyx::drawText("EasySlither", &location,
                        easyx::TEXT_CENTER | easyx::TEXT_SINGLELINE);
    }

    {
        windows::Rect location{0,
                               (height / 7 + 64 + height / 3) / 2 - 7,
                               width,
                               height};
        easyx::setFont(14);
        easyx::setTextColor(GRAY_300);
        easyx::drawText(encode(GameStore::getInstance()->getUsername()),
                        &location,
                        easyx::TEXT_CENTER | easyx::TEXT_SINGLELINE);
    }

    {
        windows::Point location{width / 2,
                                height / 3};
        this->menu->renderAt(location, image.get(), RenderAnchor::TOP_MIDDLE);
    }

    {
        windows::Rect location{8,
                               height - 24,
                               width,
                               height - 8};
        easyx::setFont(16);
        easyx::setTextColor(GRAY_100);
        easyx::drawText(encode("北京工业大学\t高级语言程序设计课设"),
                        &location,
                        easyx::TEXT_LEFT | easyx::TEXT_VCENTER | easyx::TEXT_EXPANDTABS);
    }

    {
        windows::Rect location{0,
                               height - 24,
                               width - 8,
                               height - 8};
        easyx::drawText(encode("22080206\t敬博浩"),
                        &location,
                        easyx::TEXT_RIGHT | easyx::TEXT_VCENTER | easyx::TEXT_EXPANDTABS);
    }

    easyx::setWorkingImage();

    return image;
}
