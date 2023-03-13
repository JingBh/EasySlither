/**
 * Interface of the Screen module.
 */

export module Screen;

import <list>;
import <memory>;

import ThirdParty;
import Utils.Drawable;

export enum class ScreenName {
    MAIN,
    SINGLE_PLAYER,
    MULTI_PLAYER
};

export class Screen {
private:
    int width, height;

    [[maybe_unused]] windows::WindowHandle handle;

    std::list<LocationAwareDrawable *> drawables;

public:
    Screen();

    ~Screen();

    void addElement(LocationAwareDrawable *drawable);

    void removeElement(LocationAwareDrawable *drawable);

    void refresh() const;

    int getWidth() const;

    int getHeight() const;

    windows::WindowHandle getHandle() const;
};

export class MainScreen : public LocationAwareDrawable {
protected:
    const Screen &screen;

public:
    explicit MainScreen(const Screen &screen)
        : screen{screen} {};

    [[nodiscard]] std::unique_ptr <easyx::Image> renderImage() const final;

    void renderTitle(easyx::Image *image) const;

    void renderSign(easyx::Image *image) const;

    void render() final;
};
