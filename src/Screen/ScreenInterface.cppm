/**
 * Interface of the Screen module.
 */

export module Screen;

import <list>;
import <memory>;
import <string>;

import Game;
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

    int getWidth() const;

    int getHeight() const;

    windows::WindowHandle getHandle() const;
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

export class GameScreen : public LocationAwareDrawable {
protected:
    const Screen &screen;

private:
    World *world;

public:
    GameScreen(Screen &screen, World *world)
        : screen{screen}, world{world} {}

    [[nodiscard]] std::unique_ptr <easyx::Image> renderImage() const final;
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
