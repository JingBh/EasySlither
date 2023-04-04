export module Internal.ScreenMediator;

import <cstdlib>;
import <iostream>;
import <memory>;
import <mutex>;
import <string_view>;

import Internal.GameServer;
import Internal.GameStore;
import Network.ConnectionEnd;
import Screen;
import Screen.ScreenName;
import Screen.UsernameInput;
import Utils.Drawable;
import Utils.Mediator;

export class ScreenMediator : public IMediator<ScreenName> {
private:
    std::unique_ptr <LocationAwareDrawable> currentScreen = nullptr;
    GameStore *store;
    Screen &screen;

    static inline ScreenMediator *instance_{nullptr};
    static inline std::mutex mutex_;

protected:
    explicit ScreenMediator(Screen &screen)
        : screen{screen}, store{GameStore::getInstance()} {}

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
                GameServer::start();
#ifdef DEBUG
                std::cout << "Screen switched: Game\n";
#endif //DEBUG
                this->useScreen(std::make_unique<GameScreen>(screen));
                break;

            case MULTI_PLAYER:
#ifdef DEBUG
                std::cout << "Screen switched: Multi player\n";
#endif //DEBUG
                // TODO: Server selection
#ifdef DEBUG
                std::cout << "Screen switched: Loading\n";
#endif //DEBUG
                this->useScreen(std::make_unique<LoadingScreen>(screen, "正在连接到服务器..."));
                // TODO: connect to server
#ifdef DEBUG
                std::cout << "Screen switched: Game\n";
#endif //DEBUG
                // this->useScreen(std::make_unique<GameScreen>(screen));
                break;

            case USERNAME_INPUT:
#ifdef DEBUG
                std::cout << "Popup activated: Username input\n";
#endif //DEBUG
                UsernameInput()();
                break;

            case EXIT:
                std::exit(0);
        }
    }

    void useScreen(std::unique_ptr <LocationAwareDrawable> newScreen) {
        if (currentScreen != nullptr) {
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
