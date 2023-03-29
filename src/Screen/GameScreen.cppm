export module Screen.GameScreen;

import <memory>;

import Screen;
import ThirdParty;
import Utils.Colors;

[[nodiscard]] std::unique_ptr <easyx::Image> GameScreen::renderImage() const {
    auto image = std::make_unique<easyx::Image>(
        this->screen.getWidth(), this->screen.getHeight());

    easyx::setWorkingImage(image.get());
    easyx::setBackgroundColor(BLACK);
    easyx::clearDevice();

    // TODO: Render the game

    easyx::setWorkingImage();

    return image;
}
