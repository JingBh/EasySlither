export module Internal.UserInput;

import <cmath>;
import <numbers>;
import <mutex>;

import Screen.ScaleFactor;
import ThirdParty;
import Utils.Observer;
import Utils.Throttle;

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
        mutex_.lock();
        if (instance_ == nullptr) {
            instance_ = new SubjectInputDirection();
        }
        mutex_.unlock();

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
    windows::Point lastPoint{-2, -2};

    static SubjectMouseMove *instance_;
    static std::mutex mutex_;

public:
    void update(windows::WindowHandle handle) {
        auto point = windows::getMousePosition(handle);

        if (point.x != -1 && point.y != -1) {
            // High DPI scale
            point.x = static_cast<int>(static_cast<float>(point.x) / SCALE_FACTOR);
            point.y = static_cast<int>(static_cast<float>(point.y) / SCALE_FACTOR);

            if (point.x != lastPoint.x && point.y != lastPoint.y) {
                this->notify(point);
                lastPoint = point;
            }
        }
    }

    static SubjectMouseMove *getInstance() {
        mutex_.lock();
        if (instance_ == nullptr) {
            instance_ = new SubjectMouseMove();
        }
        mutex_.unlock();

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

export enum class KeyType {
    SWITCH,
    CONFIRM,
    CANCEL,
    INFO,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

export class SubjectKeyPress : public ISubject<KeyType> {
private:
    static SubjectKeyPress *instance_;
    static std::mutex mutex_;

public:
    void update() {
        easyx::Message message;

        while (easyx::peekMessage(&message)) {
            switch (message.message) {
                case easyx::MESSAGE_LBUTTONDOWN:
                    return this->notifyAndFlush(KeyType::CONFIRM);
                case easyx::MESSAGE_RBUTTONDOWN:
                    return this->notifyAndFlush(KeyType::INFO);
                case easyx::MESSAGE_KEYDOWN:
                    switch (message.vkcode) {
                        case windows::VKEY_TAB:
                            return this->notifyAndFlush(KeyType::SWITCH);
                        case windows::VKEY_SPACE:
                        case windows::VKEY_ENTER:
                            return this->notifyAndFlush(KeyType::CONFIRM);
                        case windows::VKEY_ESCAPE:
                            return this->notifyAndFlush(KeyType::CANCEL);
                        case windows::VKEY_UP:
                            return this->notifyAndFlush(KeyType::UP);
                        case windows::VKEY_DOWN:
                            return this->notifyAndFlush(KeyType::DOWN);
                        case windows::VKEY_LEFT:
                            return this->notifyAndFlush(KeyType::LEFT);
                        case windows::VKEY_RIGHT:
                            return this->notifyAndFlush(KeyType::RIGHT);
                        default:
                            break;
                    }
                default:
                    break;
            }
        }

        windows::XinputState state;
        windows::createZeroMemory(&state, sizeof(windows::XinputState));
        const auto dwResult = windows::XinputGetState(&state);
        if (dwResult == windows::XINPUT_SUCCESS) {
            if (state.Gamepad.wButtons & windows::XINPUT_A) {
                return this->notify(KeyType::CONFIRM);
            } else if (state.Gamepad.wButtons & windows::XINPUT_B) {
                return this->notify(KeyType::CANCEL);
            } else if (state.Gamepad.wButtons & windows::XINPUT_DPAD_UP) {
                return this->notify(KeyType::UP);
            } else if (state.Gamepad.wButtons & windows::XINPUT_DPAD_DOWN) {
                return this->notify(KeyType::DOWN);
            } else if (state.Gamepad.wButtons & windows::XINPUT_DPAD_RIGHT) {
                return this->notify(KeyType::RIGHT);
            } else if (state.Gamepad.wButtons & windows::XINPUT_DPAD_LEFT) {
                return this->notify(KeyType::LEFT);
            }
        }
    }

    void notifyAndFlush(const KeyType event) {
        easyx::flushMessage();
        this->notify(event);
    }

    static SubjectKeyPress *getInstance() {
        mutex_.lock();
        if (instance_ == nullptr) {
            instance_ = new SubjectKeyPress();
        }
        mutex_.unlock();

        return instance_;
    }
};

SubjectKeyPress *SubjectKeyPress::instance_{nullptr};
std::mutex SubjectKeyPress::mutex_;

export class ObservesKeyPress : public IObserver<KeyType> {
public:
    ObservesKeyPress() {
        SubjectKeyPress::getInstance()->attach(this);
    }

    ~ObservesKeyPress() override {
        SubjectKeyPress::getInstance()->detach(this);
    }

    void update(const KeyType &message) final {
        if (throttleFor(150, "keypress")) {
            this->onKeyPress(message);
        }
    }

    virtual void onKeyPress(KeyType keyType) = 0;
};