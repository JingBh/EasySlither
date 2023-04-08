export module Internal.ScreenMediator;

import <cstdlib>;
import <iostream>;
import <memory>;
import <mutex>;
import <string_view>;

import Game.GameType;
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
    void applyChanges() final {
        if (!this->pendingChange.has_value()) {
            return;
        }

        switch (this->pendingChange.value()) {
            using
            enum ScreenName;

            case MAIN:
                this->useScreen(std::make_unique<MainScreen>(screen));
                break;

            case SINGLE_PLAYER_MAIN:
                this->useScreen(std::make_unique<MainScreenSinglePlayer>(screen));
                break;

            case SINGLE_PLAYER_START:
                this->store->lastGameType = GameType::SINGLE_PLAYER;
                GameServer::start();
                this->useScreen(std::make_unique<GameScreen>(screen));
                break;

            case SINGLE_PLAYER_RANK:
                this->useScreen(std::make_unique<RankScreen>(screen));
                break;

            case MULTI_PLAYER_MAIN:
                this->useScreen(std::make_unique<MainScreenMultiPlayer>(screen));
                break;

            case MULTI_PLAYER_START:
                this->store->lastGameType = GameType::MULTI_PLAYER;
                this->useScreen(std::make_unique<LoadingScreen>(screen, "正在连接到服务器..."));
                // TODO: connect to server
                // this->useScreen(std::make_unique<GameScreen>(screen));
                break;

            case GAME_OVER:
                GameServer::stop();
                this->useScreen(std::make_unique<GameOverScreen>(screen));
                break;

            case USERNAME_INPUT:
                UsernameInput()();
                break;

            case SERVER_INPUT:
                // TODO: Server selection
                break;

            case EXIT:
                std::exit(0);
        }

        IMediator<ScreenName>::applyChanges();
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
