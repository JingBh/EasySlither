export module Internal.ScreenMediator;

import <cstdlib>;
import <iostream>;
import <memory>;
import <mutex>;
import <string_view>;

import Screen;
import Screen.ScreenName;
import Screen.UsernameInput;
import Utils.Drawable;
import Utils.Mediator;

export class ScreenMediator : public IMediator<ScreenName> {
private:
    std::unique_ptr <LocationAwareDrawable> currentScreen = nullptr;
    Screen &screen;

    static ScreenMediator *instance_;
    static std::mutex mutex_;

protected:
    explicit ScreenMediator(Screen &screen)
        : screen{screen} {}

public:
    void notify(const ScreenName &screenName) final {
        switch (screenName) {
            using
            enum ScreenName;

            case MAIN:
#ifdef DEBUG
                std::cout << "Screen switched: Main\n";
#endif //DEBUG
                this->useScreen(std::make_unique<MainScreen>(screen));
                break;

            case SINGLE_PLAYER:
#ifdef DEBUG
                std::cout << "Screen switched: Loading\n";
#endif //DEBUG
                this->useScreen(std::make_unique<LoadingScreen>(screen, "正在加载世界..."));
                // TODO
                break;

            case MULTI_PLAYER:
#ifdef DEBUG
                std::cout << "Screen switched: Multi player\n";
#endif //DEBUG
                // TODO
#ifdef DEBUG
                std::cout << "Screen switched: Loading\n";
#endif //DEBUG
                this->useScreen(std::make_unique<LoadingScreen>(screen, "正在连接到服务器..."));
                // TODO
                break;

            case USERNAME_INPUT:
#ifdef DEBUG
                std::cout << "Popup activated: Username input\n";
#endif //DEBUG
                usernameInput();
                break;

            case EXIT:
                std::exit(0);
        }
    }

    void useScreen(std::unique_ptr <LocationAwareDrawable> newScreen) {
        if (currentScreen.get() != nullptr) {
            screen.removeElement(currentScreen.get());
        }

        screen.addElement(newScreen.get());
        currentScreen = std::move(newScreen);
    }

    static ScreenMediator *getInstance(Screen *screen = nullptr) {
        mutex_.lock();
        if (instance_ == nullptr && screen != nullptr) {
            instance_ = new ScreenMediator(*screen);
        }
        mutex_.unlock();

        return instance_;
    }
};

ScreenMediator *ScreenMediator::instance_{nullptr};
std::mutex ScreenMediator::mutex_;
