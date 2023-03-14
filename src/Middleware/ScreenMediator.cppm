export module Middleware.ScreenMediator;

import <iostream>;
import <mutex>;
import <string_view>;

import Screen;
import Screen.ScreenName;
import Utils.Drawable;
import Utils.Mediator;

export class ScreenMediator : public IMediator<ScreenName> {
    using
    enum ScreenName;

private:
    LocationAwareDrawable *currentScreen = nullptr;
    Screen &screen;

    static ScreenMediator *instance_;
    static std::mutex mutex_;

protected:
    explicit ScreenMediator(Screen &screen)
        : screen{screen} {}

public:
    void notify(const ScreenName &screenName) final {
        switch (screenName) {
            case MAIN:
#ifdef DEBUG
                std::cout << "Screen switched: Main\n";
#endif //DEBUG
                this->useScreen(new MainScreen(screen));
                break;
            case SINGLE_PLAYER:
#ifdef DEBUG
                std::cout << "Screen switched: Single player\n";
#endif //DEBUG
                // TODO
                break;
            case MULTI_PLAYER:
#ifdef DEBUG
                std::cout << "Screen switched: Multi player\n";
#endif //DEBUG
                // TODO
                break;
        }
    }

    void useScreen(LocationAwareDrawable *newScreen) {
        screen.removeElement(currentScreen);
        screen.addElement(newScreen);
        delete currentScreen;
        currentScreen = newScreen;
    }

    static ScreenMediator *getInstance(Screen *screen = nullptr) {
        [[maybe_unused]] std::lock_guard <std::mutex> lock(mutex_);
        if (instance_ == nullptr && screen != nullptr) {
            instance_ = new ScreenMediator(*screen);
        }

        return instance_;
    }
};

ScreenMediator *ScreenMediator::instance_{nullptr};
std::mutex ScreenMediator::mutex_;
