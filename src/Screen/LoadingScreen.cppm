export module Screen.LoadingScreen;

import <memory>;

import Screen;
import ThirdParty;
import Utils.Colors;
import Utils.TextEncode;

[[nodiscard]] std::unique_ptr <easyx::Image> LoadingScreen::renderImage() const {
    auto image = std::make_unique<easyx::Image>(
        this->screen.getWidth(), this->screen.getHeight());

    easyx::setWorkingImage(image.get());
    easyx::setBackgroundColor(GRAY_900);
    easyx::clearDevice();

    windows::Rect titleLocation{0, 0,
                                image->getwidth(),
                                image->getheight()};
    easyx::setFont(16);
    easyx::drawText(encode(this->title),
                    &titleLocation,
                    easyx::TEXT_CENTER | easyx::TEXT_VCENTER | easyx::TEXT_SINGLELINE);

    return image;
}
