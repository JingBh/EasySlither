export module Middleware.UserInput;

import <cmath>;
import <numbers>;
import <mutex>;

import Screen;
import ThirdParty;
import Utils.Observer;

export class SubjectInputDirection : public ISubject<int> {
private:
    static SubjectInputDirection *instance_;
    static std::mutex mutex_;

public:
    void update() {
        int degree;

        /**
         * Xbox Controller input
         */
        windows::XinputState state;
        windows::createZeroMemory(&state, sizeof(windows::XinputState));
        const auto dwResult = windows::XinputGetState(&state);
        if (dwResult == windows::XINPUT_SUCCESS) {
            const auto leftThumbX = state.Gamepad.sThumbLX;
            const auto leftThumbY = state.Gamepad.sThumbLY;

            if (leftThumbX < windows::XINPUT_LEFT_THUMB_DEADZONE &&
                leftThumbX > -windows::XINPUT_LEFT_THUMB_DEADZONE &&
                leftThumbY < windows::XINPUT_LEFT_THUMB_DEADZONE &&
                leftThumbY > -windows::XINPUT_LEFT_THUMB_DEADZONE) {

                const auto leftThumbMagnitude = std::sqrt(
                    leftThumbX * leftThumbX + leftThumbY * leftThumbY);

                const auto normalizedLX = leftThumbX / leftThumbMagnitude;
                const auto normalizedLY = leftThumbY / leftThumbMagnitude;

                auto theta = std::atan2(normalizedLY, normalizedLX);
                if (theta < 0.0) {
                    theta += 2 * std::numbers::pi;
                }

                degree = static_cast<int>(theta * 180.0 / std::numbers::pi);
                return this->notify(degree);
            }

            if (state.Gamepad.wButtons & windows::XINPUT_DPAD_UP
                && state.Gamepad.wButtons & windows::XINPUT_DPAD_RIGHT) {
                degree = 45;
            } else if (state.Gamepad.wButtons & windows::XINPUT_DPAD_UP
                       && state.Gamepad.wButtons & windows::XINPUT_DPAD_LEFT) {
                degree = 315;
            } else if (state.Gamepad.wButtons & windows::XINPUT_DPAD_DOWN
                       && state.Gamepad.wButtons & windows::XINPUT_DPAD_RIGHT) {
                degree = 135;
            } else if (state.Gamepad.wButtons & windows::XINPUT_DPAD_DOWN
                       && state.Gamepad.wButtons & windows::XINPUT_DPAD_LEFT) {
                degree = 225;
            } else if (state.Gamepad.wButtons & windows::XINPUT_DPAD_UP) {
                degree = 0;
            } else if (state.Gamepad.wButtons & windows::XINPUT_DPAD_DOWN) {
                degree = 180;
            } else if (state.Gamepad.wButtons & windows::XINPUT_DPAD_RIGHT) {
                degree = 90;
            } else if (state.Gamepad.wButtons & windows::XINPUT_DPAD_LEFT) {
                degree = 270;
            } else {
                degree = 360; // flag indicates that shouldn't return
            }

            if (degree < 360) {
                return this->notify(degree);
            }
        }

        /**
         * Keyboard input
         */
        const bool isKeyUp = windows::isKeyDown(windows::VKEY_UP)
                             || windows::isKeyDown('W');
        const bool isKeyDown = windows::isKeyDown(windows::VKEY_DOWN)
                               || windows::isKeyDown('S');
        const bool isKeyLeft = windows::isKeyDown(windows::VKEY_LEFT)
                               || windows::isKeyDown('A');
        const bool isKeyRight = windows::isKeyDown(windows::VKEY_RIGHT)
                                || windows::isKeyDown('D');

        if (isKeyUp && isKeyRight) {
            degree = 45;
        } else if (isKeyUp && isKeyLeft) {
            degree = 315;
        } else if (isKeyDown && isKeyRight) {
            degree = 135;
        } else if (isKeyDown && isKeyLeft) {
            degree = 225;
        } else if (isKeyUp) {
            degree = 0;
        } else if (isKeyDown) {
            degree = 180;
        } else if (isKeyRight) {
            degree = 90;
        } else if (isKeyLeft) {
            degree = 270;
        } else {
            degree = 360; // flag indicates that shouldn't return
        }

        if (degree < 360) {
            return this->notify(degree);
        }
    }

    static SubjectInputDirection *getInstance() {
        [[maybe_unused]] std::lock_guard <std::mutex> lock(mutex_);
        if (instance_ == nullptr) {
            instance_ = new SubjectInputDirection();
        }

        return instance_;
    }
};

SubjectInputDirection *SubjectInputDirection::instance_{nullptr};
std::mutex SubjectInputDirection::mutex_;

export class ObservesInputDirection : public IObserver<int> {
public:
    ObservesInputDirection() {
        SubjectInputDirection::getInstance()->attach(this);
    }

    ~ObservesInputDirection() override {
        SubjectInputDirection::getInstance()->detach(this);
    }

    void update(const int &message) final {
        this->onInputDirection(message);
    }

    virtual void onInputDirection(int degree) = 0;
};

export class SubjectMouseMove : public ISubject<windows::Point> {
private:
    const Screen &mScreen;

    static SubjectMouseMove *instance_;
    static std::mutex mutex_;

protected:
    explicit SubjectMouseMove(const Screen &screen)
        : mScreen{screen} {}

public:
    void update() {
        auto point = windows::getMousePosition(mScreen.getHandle());

        if (point.x != -1 && point.y != -1) {
            this->notify(point);
        }
    }

    static SubjectMouseMove *getInstance(const Screen *screen = nullptr) {
        [[maybe_unused]] std::lock_guard <std::mutex> lock(mutex_);
        if (instance_ == nullptr && screen != nullptr) {
            instance_ = new SubjectMouseMove(*screen);
        }

        return instance_;
    }
};

SubjectMouseMove *SubjectMouseMove::instance_{nullptr};
std::mutex SubjectMouseMove::mutex_;

export class ObservesMouseMove : public IObserver<windows::Point> {
public:
    ObservesMouseMove() {
        SubjectMouseMove::getInstance()->attach(this);
    }

    ~ObservesMouseMove() override {
        SubjectMouseMove::getInstance()->detach(this);
    }

    void update(const windows::Point &message) final {
        this->onMouseMove(message);
    }

    virtual void onMouseMove(const windows::Point &point) = 0;
};
