/**
 * Interface of the Screen module.
 */

export module Screen;

import <list>;
import <memory>;
import <string>;

import Internal.GameStore;
import Internal.UserInput;
import Menu;
import Screen.ScreenName;
import ThirdParty;
import Utils.Drawable;

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

    [[nodiscard]] int getWidth() const;

    [[nodiscard]] int getHeight() const;

    [[nodiscard]] int getUnscaledWidth() const;

    [[nodiscard]] int getUnscaledHeight() const;

    [[nodiscard]] windows::WindowHandle getHandle() const;
};

export class MainScreen : public LocationAwareDrawable {
protected:
    const Screen &screen;

private:
    // the menu object stored in a pointer
    // as a workaround for renderImage is const
    std::unique_ptr <Menu> menu;

public:
    explicit MainScreen(Screen &screen);

    [[nodiscard]] std::unique_ptr <easyx::Image> renderImage() const final;
};

export class GameScreen : public LocationAwareDrawable,
                          public ObservesInputDirection, public ObservesMouseMove, public ObservesKeyHold {
protected:
    const Screen &screen;

private:
    GameStore *store;
    int zoom = 12;

public:
    GameScreen(Screen &screen)
        : screen{screen}, store{GameStore::getInstance()} {}

    ~GameScreen() override {
        ObservesInputDirection::~ObservesInputDirection();
        ObservesMouseMove::~ObservesMouseMove();
        ObservesKeyHold::~ObservesKeyHold();
    }

    [[nodiscard]] std::unique_ptr <easyx::Image> renderImage() const final;

protected:
    void onInputDirection(int degree) final;

    void onMouseMove(const windows::Point &point) final;

    void onKeyHold(bool hold) final;
};

export class LoadingScreen : public LocationAwareDrawable {
protected:
    const Screen &screen;

private:
    std::string title;

public:
    LoadingScreen(const Screen &screen, std::string title)
        : screen{screen}, title{title} {}

    [[nodiscard]] std::unique_ptr <easyx::Image> renderImage() const final;
};
