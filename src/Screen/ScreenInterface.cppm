/**
 * Interface of the Screen module.
 */

export module Screen;

import <list>;
import <memory>;
import <string>;

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
private:
    // the menu object stored in a pointer
    // as a workaround for renderImage is const
    std::unique_ptr <Menu> menu;

protected:
    const Screen &screen;

public:
    explicit MainScreen(Screen &screen);

    [[nodiscard]] std::unique_ptr <easyx::Image> renderImage() const final;

    void render() final;
};

export class LoadingScreen : public LocationAwareDrawable {
private:
    std::string title;

protected:
    const Screen &screen;

public:
    LoadingScreen(const Screen &screen, std::string title)
        : screen{screen}, title{title} {}

    [[nodiscard]] std::unique_ptr <easyx::Image> renderImage() const final;

    void render() final;
};